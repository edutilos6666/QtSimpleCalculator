#include "personwindow.h"
#include "ui_personwindow.h"
#include "ui_personreportwindow.h"
PersonWindow::PersonWindow(QWidget *parent, PersonDAOImpl *dao):
    QMainWindow(parent), dao(dao), ui(new Ui::PersonWindow)
{
ui->setupUi(this);
configureWindow();
registerEvents();
}

PersonWindow::~PersonWindow()
{
    delete ui;
}

void PersonWindow::refresh()
{
     int rowCount = personModel->rowCount();
     personModel->removeRows(0 , rowCount);
     QList<shared_ptr<Person>> all = dao->findAll();
     int row = 0;
     for(auto&p: all) {
         QStandardItem* item = new QStandardItem(QString::number(p->id));
         personModel->setItem(row, 0, item);
         item = new QStandardItem(p->name);
         personModel->setItem(row, 1, item);
         item = new QStandardItem(QString::number(p->age));
         personModel->setItem(row, 2, item);
         item = new QStandardItem(QString::number(p->wage));
         personModel->setItem(row, 3, item);
         QString activeStr = p->active?"true":"false";
         item = new QStandardItem(activeStr);
         personModel->setItem(row, 4, item);
         ++row;
     }
}

void PersonWindow::configureWindow()
{
    personModel = new QStandardItemModel();
    ui->personView->setModel(personModel);
    personModel->setHorizontalHeaderLabels({"Id", "Name", "Age", "Wage", "Active"});
    refresh();

    mapper = new QDataWidgetMapper();
    mapper->setModel(personModel);
    mapper->addMapping(ui->editId, 0);
    mapper->addMapping(ui->editName, 1);
    mapper->addMapping(ui->editAge, 2);
    mapper->addMapping(ui->editWage, 3);
    mapper->addMapping(ui->editActive, 4);
    mapper->toFirst();
}

void PersonWindow::registerEvents()
{
    QObject::connect(ui->personView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    QObject::connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
    QObject::connect(ui->btnPrevious, SIGNAL(clicked(bool)), this, SLOT(previous()));
    QObject::connect(ui->btnNext, SIGNAL(clicked(bool)), this, SLOT(next()));
   QObject::connect(ui->actionCreate, SIGNAL(triggered(bool)), this , SLOT(createNewPerson()));
   QObject::connect(ui->actionRemove, SIGNAL(triggered(bool)), this , SLOT(deleteSelectedPerson()));
  // Ui::PersonWindow* uiTemp = ui;
 //  qDebug() << (*ui) ;


   QObject::connect(ui->btnCreate, &QPushButton::clicked, [this](){
     this->createReport();
   });
}


void PersonWindow::createReport()
{
    //qDebug()<< uiTemp ;

    QString idStr = ui->editId->text(),
            nameStr = ui->editName->text(),
            ageStr = ui->editAge->text(),
            wageStr = ui->editWage->text(),
            activeStr = ui->editActive->text();
        PersonReportWindow *reportWindow = new PersonReportWindow();

        qDebug() << reportWindow->ui->editId;
        reportWindow->ui->editId->setText(idStr);
         qDebug() << "insdide btnCreate lambda";
        reportWindow->ui->editName->setText(nameStr);
        reportWindow->ui->editAge->setText(ageStr);
        reportWindow->ui->editWage ->setText(wageStr);
        reportWindow->ui->editActive->setText(activeStr);
        QObject::connect(reportWindow->ui->btnClose, &QPushButton::clicked, [=](){
            reportWindow->close();
        });
        reportWindow->show();
}


void PersonWindow::previous()
{
    mapper->toPrevious();
}

void PersonWindow::next()
{
    mapper->toNext();
}

void PersonWindow::updateButtons(int row)
{
    ui->btnNext->setEnabled(row < personModel->rowCount()-1);
    ui->btnPrevious->setEnabled(row > 0);
}

void PersonWindow::createNewPerson()
{
    QMainWindow *secondWindow = new QMainWindow();
    QWidget *widget =new QWidget(secondWindow);
    secondWindow->setCentralWidget(widget);

    QFormLayout *layout = new QFormLayout();
    widget->setLayout(layout);

    secondWindow->resize(400, 300);
    secondWindow->setWindowTitle("Create New Person");
    //id
    QLabel *lblId = new QLabel("Id: ");
    QLineEdit *editId = new QLineEdit();
    layout->addRow(lblId, editId);
    //name
    QLabel *lblName = new QLabel("Name: ");
    QLineEdit *editName = new QLineEdit();
    layout->addRow(lblName, editName);
    //age
    QLabel *lblAge = new QLabel("Age: ");
    QLineEdit *editAge = new QLineEdit();
    layout->addRow(lblAge, editAge);
    //wage
    QLabel *lblWage = new QLabel("Wage: ");
    QLineEdit *editWage = new QLineEdit();
    layout->addRow(lblWage, editWage);
    //active
    QLabel *lblActive = new QLabel("Active: ");
    QLineEdit *editActive = new QLineEdit();
    layout->addRow(lblActive, editActive);
    //buttons
    QPushButton *btnSave = new QPushButton("Save");
    QPushButton *btnCancel = new QPushButton("Cancel");
    layout->addRow(btnSave, btnCancel);

    QObject::connect(btnSave, &QPushButton::clicked, [=](){

       try {
            qDebug() << "editId" ;
        QString idStr = editId->text();
         qDebug() << "inside lambda" ;
                QString nameStr = editName->text(),
                ageStr = editAge->text(),
                wageStr = editWage->text(),
                activeStr = editActive->text();
        QRegExp regex;
        regex.setPattern("\\d+");
        bool success = true;
        QString errorMsg ;
        QTextStream stream(&errorMsg);


        if(!regex.exactMatch(idStr)) {
            stream << "Id must contain only digits.\n";
            success = false ;
        }
        regex.setPattern("(\\w+|\\d+)");
        if(!regex.exactMatch(nameStr))  {
            stream << "Name must contain letters and digits.\n";
            success = false;
        }

        regex.setPattern("\\d+");
        if(!regex.exactMatch(ageStr)) {
            stream << "Age must contain only digits.\n";
            success = false;
        }

        regex.setPattern("\\d+.\\d*");
        if(!regex.exactMatch(wageStr)) {
            stream << "Wage must be positive floating point number.\n";
            success = false;
        }

        regex.setPattern("(true|false)");
        if(!regex.exactMatch(activeStr)) {
            stream << "Active must be either true or false.\n";
            success = false;
        }



        if(!success) {
            qDebug() << errorMsg;
            QMessageBox *msgBox = new QMessageBox(secondWindow);
            msgBox->setWindowTitle("error occurred");
            msgBox->setText(errorMsg);
            msgBox->show();
        } else  {
            long id = idStr.toLong();
            int age = ageStr.toInt();
            double wage = wageStr.toDouble();
            bool active = (activeStr=="true")?true: false;
            dao->save(shared_ptr<Person>(new Person(id, nameStr,age,  wage, active)));
            refresh();
            secondWindow->close();
        }

        } catch(exception &ex) {
             qDebug() << ex.what();
        }
    });


    secondWindow->show();
}

void PersonWindow::deleteSelectedPerson()
{
      try {
     auto indices = ui->personView->selectionModel()->selectedIndexes();
     if(indices.count() <= 0) return;
     QModelIndex selectedIndex = indices[0];
      //QStandardItem* item = personModel->item(selectedIndex.row(), 0);
      QVariant data = personModel->data(personModel->index(selectedIndex.row(), 0));
    //  qDebug() << "row , col = " << selectedIndex.row() << ", " << selectedIndex.column();
      //qDebug() << item->data();
      long id = data.toLongLong();
   qDebug() << "id = " << id;
          dao->remove(id);
          refresh();
      } catch(exception &ex) {
          qDebug()<< ex.what();
    }
}



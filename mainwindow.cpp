#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    registerEvents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::registerEvents()
{
   QObject::connect(ui->btn0, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn1, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn2, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn3, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn4, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn5, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn6, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn7, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn8, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btn9, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btnAdd, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btnSubtract, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btnMultiply, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btnDivide, SIGNAL(clicked(bool)), this, SLOT(handleInput()));
   QObject::connect(ui->btnEqual, SIGNAL(clicked(bool)), this, SLOT(handleCalculate()));
   QObject::connect(ui->btnClear, SIGNAL(clicked(bool)), this, SLOT(clearDisplay()));
}

void MainWindow::handleInput()
{
    auto sender = QObject::sender();
    QString input ="" ;
    if(sender == ui->btn0) {
      input = "0";
    } else if(sender == ui->btn1) {
     input = "1";
    } else if(sender == ui->btn2) {
        input = "2";
    } else if(sender == ui->btn3) {
        input = "3";
    } else if(sender == ui->btn4) {
        input = "4";
    } else if(sender == ui->btn5) {
        input = "5";
    } else if(sender == ui->btn6) {
        input = "6";
    } else if(sender == ui->btn7) {
        input = "7";
    } else if(sender == ui->btn8) {
        input = "8";
    } else if(sender == ui->btn9) {
        input = "9";
    } else if(sender == ui->btnAdd)  {
        input = " + ";
    } else if(sender == ui->btnSubtract) {
        input = " - ";
    } else if(sender == ui->btnMultiply) {
        input = " * ";
    } else if(sender == ui->btnDivide) {
        input = " / ";
    }


    QString text = ui->editDisplay->toPlainText();
    if(text.count() == 0)  {
        ui->editDisplay->setText(input);
        return ;
    }
    QChar lastCh =  text[text.count()-1];
    if((lastCh == '+' || lastCh == '-' || lastCh == '*' || lastCh == '/')) {
         input = input.prepend(' ');
    }
    text = text + input;
    ui->editDisplay->clear();
    ui->editDisplay->setText(text);
}

void MainWindow::handleCalculate()
{
   QString text = ui->editDisplay->toPlainText();
    QRegExp regex("\\s+");
    QStringList list = text.split(regex);
    QStack<QString> tempOperators ;
    QStack<QString> tempOperands;

    for(auto& str: list) {
        qDebug() << str;
    }
   try  {
        regex.setPattern("\\d+");
        for(auto& str: list) {
            if(str == "+" || str == "-" || str == "*" || str == "/") {
              tempOperators.push(str);
            } else if(regex.exactMatch(str)){
               tempOperands.push(str);
            } else  {
                throw exception("invalid arithmetic expression");
            }

        }



        QStack<QString> operands ;
        QStack<QString> operators;
        while(!tempOperands.isEmpty()) {
            operands.push(tempOperands.pop());
        }

        while(!tempOperators.isEmpty()) {
            operators.push(tempOperators.pop());
        }


        double res = 0.0;
        int counter = 0;
        while(!operators.isEmpty()) {
            QString op = operators.pop();
            QString str1 = operands.pop();
            QString str2 = operands.pop();
            double n1 = str1.toDouble(),
                    n2 = str2.toDouble();
            if(op == "+") res = n1 + n2;
            else if(op == "-") res = n1 - n2 ;
            else if(op == "*") res = n1 * n2 ;
            else if(op == "/") res = n1 / n2 ;
            operands.push(QString::number(res));
        }

        qDebug() << "res = " << res;
   } catch(exception& ex) {
     qDebug() <<ex.what();
     QMessageBox* msgBox = new QMessageBox();
     msgBox->setWindowTitle("Exception occurred.");
     msgBox->setText(ex.what());
     msgBox->show();
   }
}

void MainWindow::clearDisplay()
{
    ui->editDisplay->setText("");
}

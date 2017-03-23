#ifndef PERSONWINDOW_H
#define PERSONWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <memory>
#include "person.h"
#include <QList>
#include <QDataWidgetMapper>
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExp>
#include <QMessageBox>
#include <QTextStream>
#include "personreportwindow.h"

using namespace std;


namespace Ui {
class PersonWindow;
}

class PersonWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PersonWindow(QWidget *parent = 0, PersonDAOImpl *dao= 0);
    ~PersonWindow();
    void refresh();
    void configureWindow();
    void registerEvents();
public slots:
   void previous();
   void next();
   void updateButtons(int row);
   void createNewPerson();
   void deleteSelectedPerson();
   void createReport();
public:
    Ui::PersonWindow *ui;
    QStandardItemModel* personModel;
    PersonDAOImpl * dao;
    QDataWidgetMapper *mapper ;
};

#endif // PERSONWINDOW_H

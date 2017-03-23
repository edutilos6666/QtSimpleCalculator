#include "personreportwindow.h"
#include "ui_personreportwindow.h"

PersonReportWindow::PersonReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PersonReportWindow)
{
    ui->setupUi(this);
}

PersonReportWindow::~PersonReportWindow()
{
    delete ui;
}

QLineEdit *PersonReportWindow::getId()
{
    return 0;
}

QLineEdit *PersonReportWindow::getName()
{
return ui->editId;
}

QLineEdit *PersonReportWindow::getAge()
{
return ui->editAge;
}

QLineEdit *PersonReportWindow::getWage()
{
return ui->editWage;
}

QLineEdit *PersonReportWindow::getActive()
{
return ui->editActive;
}

QPushButton *PersonReportWindow::getBtnCLose()
{
return ui->btnClose;
}

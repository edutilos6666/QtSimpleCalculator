#ifndef PERSONREPORTWINDOW_H
#define PERSONREPORTWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>


namespace Ui {
class PersonReportWindow;
}

class PersonReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PersonReportWindow(QWidget *parent = 0);
    ~PersonReportWindow();

public:
    Ui::PersonReportWindow *ui;
    QLineEdit* getId();
    QLineEdit* getName();
    QLineEdit* getAge();
    QLineEdit* getWage();
    QLineEdit* getActive();
    QPushButton* getBtnCLose();
};

#endif // PERSONREPORTWINDOW_H

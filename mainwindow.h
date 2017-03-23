#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <iostream>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QRegExp>
#include <QStack>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void registerEvents();

public slots:
    void handleInput();
    void handleCalculate();
    void clearDisplay();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;

/*namespace Ui {
class MainWindow;
}*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //QLCDNumber::QLCDNumber *screen;
    QPushButton *plus;
    QPushButton *minus;
    QPushButton *equal;
    QPushButton *clear;
    QPushButton *btn[16];
    /*QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    QPushButton *btn6;
    QPushButton *btn7;
    QPushButton *btn8;
    QPushButton *btn9;
    QPushButton *btnA;
    QPushButton *btnB;
    QPushButton *btnC;
    QPushButton *btnD;
    QPushButton *btnE;
    QPushButton *btnF;*/
};

#endif // MAINWINDOW_H

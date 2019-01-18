#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //screen = new QLCDNumber::QLCDNumber();//QWidget * parent = 0);
    for (int i=0; i<16; i++){
        btn[i] = new QPushButton("%X",i);
    }

    // LAYOUTS
    QHBoxLayout *signs = new QHBoxLayout;
    signs->addWidget(plus);
    signs->addWidget(minus);
    signs->addWidget(equal);
    signs->addWidget(clear);

    QHBoxLayout *hButtons[4];
    for (int i=0; i<4; i++){
        hButtons[i] = new QHBoxLayout;
        hButtons[i]->addWidget(btn[4*i]);
        hButtons[i]->addWidget(btn[4*i + 1]);
        hButtons[i]->addWidget(btn[4*i + 2]);
        hButtons[i]->addWidget(btn[4*i + 3]);
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addLayout(screen);
    mainLayout->addLayout(signs);
    for (int i=0; i<4; i++){
        mainLayout->addLayout(hButtons[i]);
    }

    setLayout(mainLayout);
    setWindowTitle("HeXadecimal Calculator");

}

MainWindow::~MainWindow()
{

}

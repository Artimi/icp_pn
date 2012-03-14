/**
  * @file main.cpp
  * @author xsebek02 xsimon14
  * @brief Tento soubor spouští aplikaci Qt a předává jí řízení
  */

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

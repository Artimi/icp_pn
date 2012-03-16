/**
  * @file main.cpp
  * @author xsebek02 xsimon14
  * @brief Tento soubor spouští aplikaci Qt a předává jí řízení
  */

/**
  * @mainpage Dokumentace k editoru a simulátoru vysokoúrovňových Petriho sítí
  *
  * \section intro_sec Úvod
  *
  * \section use_sec Použití
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

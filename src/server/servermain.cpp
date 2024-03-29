/**
  * @file servermain.cpp
  *
  * @brief Main soubor serveru
  * @author xsebek02 xsimon14
  */
#include <QtCore/QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer();
    
    return a.exec();
}

#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

void Server::startServer()
{
    if(!this->listen(QHostAddress::Any,22334))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening...";
    }
}

void Server::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << "Connecting...";
    Thread *thread = new Thread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

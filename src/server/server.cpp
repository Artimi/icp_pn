#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
}

void Server::incomingConnection(int socketDescriptor)
{
    Thread *thread = new Thread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

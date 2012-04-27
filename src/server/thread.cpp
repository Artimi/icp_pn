#include "thread.h"

Thread::Thread(int socketDescriptor, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketDescriptor;
}

void Thread::run()
{
    qDebug() <<  socketDescriptor <<"Starting thread";
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << socketDescriptor << "Client connected";

    exec();
}

void Thread::readyRead()
{
    QByteArray data = socket->readAll();

    qDebug() << socketDescriptor << "Data in:" << data;

    socket->write(data);
}

void Thread::disconnected()
{
    qDebug() << socketDescriptor << "Disconected";
    socket->deleteLater();
    exit(0);
}

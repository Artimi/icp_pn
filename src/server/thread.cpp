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
//    Message message;

//    DiagramScene scene(NULL,NULL,NULL);

//    XMLHandler xmlhandler(&scene,&message);
//    xmlhandler.readMessage(QString(data));

//    QString out = xmlhandler.writeMessage(); //jen zkouším parsing, mělo by se vrátit cojsem poslal

//    data = out.toLatin1();

//    qDebug() << socketDescriptor << "Data out:" << data;
    socket->write(data);
}

void Thread::disconnected()
{
    qDebug() << socketDescriptor << "Disconected";
    socket->deleteLater();
    exit(0);
}

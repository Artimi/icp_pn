#include "thread.h"

Thread::Thread(int socketDescriptor, QObject *parent) :
    QThread(parent)
{
    mySocketDescriptor = socketDescriptor;
}

void Thread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(mySocketDescriptor))
    {
        emit error(tcpSocket.error());
        return;
    }
}

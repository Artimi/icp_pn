#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtNetwork/QTcpSocket>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(int socketDescriptor,QObject *parent = 0);
    
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    
private:
    int mySocketDescriptor;


public slots:
    
};

#endif // THREAD_H

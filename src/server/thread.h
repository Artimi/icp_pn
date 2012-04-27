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

public slots:
    void readyRead();
    void disconnected();

private:
    int socketDescriptor;
    QTcpSocket *socket;


public slots:

};

#endif // THREAD_H

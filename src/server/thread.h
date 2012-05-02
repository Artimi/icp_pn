#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtNetwork/QTcpSocket>
#include "../pn/xmlhandler.h"
#include "../pn/message.h"
#include "manipulatenet.h"
#include "petrinet.h"
#include <QCryptographicHash>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(int socketDescriptor,QObject *parent = 0);

    void run();
    QString getUsername()
    {   return username; }
    bool authenticate(QString username, QString passwd);
signals:
    void error(QTcpSocket::SocketError socketError);

public slots:
    //void readyRead();
    void disconnected();
    void handleRequest();

private:
    int socketDescriptor;
    QTcpSocket *socket;
    QString username;

public slots:

};

#endif // THREAD_H

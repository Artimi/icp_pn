#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtNetwork/QTcpSocket>
#include "xmlhandler.h"
#include "../pn/message.h"
#include "manipulatenet.h"
#include "petrinet.h"
#include <QCryptographicHash>
#include <QMutex>
#include <QTime>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(int socketDescriptor,QObject *parent = 0);

    void run();
    QString getPath()
    {   return apppath; }
    QString getUsername()
    {   return username; }
    bool authenticate(QString username, QString passwd);
    void writeLog(QString event);
signals:
    void error(QTcpSocket::SocketError socketError);

public slots:
    void disconnected();
    void handleRequest();

private:
    QMutex mutex;
    int socketDescriptor;
    QTcpSocket *socket;
    QString username;
    QString apppath;

public slots:

};

#endif // THREAD_H

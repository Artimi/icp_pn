/**
  * @file thread.h
  *
  * @author xsebek02, xsimon14
  */
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
#include <QDateTime>

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
    void writeLogLogin();
    void writeLogWronglogin(QString name);
//    void writeLogServerStart();
    void writeLogConnect();
    void writeLogDisconnect();
    void writeLogSimulate(QString netName, QString version, QString state);
    void writeLogSave(QString netName, QString state);
    void writeLogLoad(QString netName, QString state);
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

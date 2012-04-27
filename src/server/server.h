#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QThreadPool>
#include <QDebug>

#include "thread.h"

class QTcpServer;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void startServer();

private slots:
    
signals:
    
public slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    
};

#endif // SERVER_H

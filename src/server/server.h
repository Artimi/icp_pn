#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork/QTcpServer>

#include "thread.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

private slots:
    void incomingConnection(int socketDescriptor);

    
signals:
    
public slots:
    
};

#endif // SERVER_H

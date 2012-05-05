/**
  * @file connect.h
  * @author xsebek02 xsimon14
  */
#ifndef CONNECT_H
#define CONNECT_H

#include <QDialog>
#include <QtNetwork/QAbstractSocket>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>
#include <QHostAddress>
#include <QHostInfo>

namespace Ui {
class Connect;
}

class Connect : public QDialog
{
    Q_OBJECT
    
public:
    explicit Connect(QTcpSocket *socket,QWidget *parent = 0);
    ~Connect();

public slots:
    void connectToServer();
    void gotIP(QHostInfo info);
    
private:
    Ui::Connect *ui;
    QTcpSocket *mySocket;

    int myPort;
};

#endif // CONNECT_H

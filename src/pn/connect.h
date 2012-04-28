#ifndef CONNECT_H
#define CONNECT_H

#include <QDialog>
#include <QtNetwork/QAbstractSocket>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>
#include <QHostAddress>

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
    
private:
    Ui::Connect *ui;
    QTcpSocket *mySocket;
};

#endif // CONNECT_H

#include "connect.h"
#include "ui_connect.h"

Connect::Connect(QTcpSocket *socket,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect)
{
    ui->setupUi(this);
    mySocket = socket;

    connect(ui->PBconnect,SIGNAL(clicked()),this,SLOT(connectToServer()));
}

Connect::~Connect()
{
    delete ui;
}

void Connect::connectToServer()
{
    QString strAddr = ui->LEServer->text();
    int port = ui->SBPort->value();

    QHostAddress addr;
    if(!addr.setAddress(strAddr))
    {
        QMessageBox::warning(this,
                             tr("Invalid IP adress"),
                             tr("You have entered an invalid IP adress!"));
        return;
    }
    mySocket->connectToHost(addr,port);
}

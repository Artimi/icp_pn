/**
  * @file connect.cpp
  * @author xsebek02 xsimon14
  * @brief Tento soubor udržuje a manipuluje s připojením k serveru
  */
#include "connect.h"
#include "ui_connect.h"

/**
  * Konstruktor třídy Connect
  */
Connect::Connect(QTcpSocket *socket,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect)
{
    ui->setupUi(this);
    mySocket = socket;

    connect(ui->PBconnect,SIGNAL(clicked()),this,SLOT(connectToServer()));
}

/**
  * Desktuktor třídy Connect
  */
Connect::~Connect()
{
    delete ui;
}

/**
  * Zažádá o vyhodnocení doménového jména, když je znám zavolá se gotIP()
  */
void Connect::connectToServer()
{
    QString strAddr = ui->LEServer->text();
    myPort = ui->SBPort->value();

    QHostInfo::lookupHost(strAddr,this,SLOT(gotIP(QHostInfo)));


}

/**
  * Provede vlastní připojení na server
  * @param info informace o připojovaném serveru
  */
void Connect::gotIP(QHostInfo info)
{
    QHostAddress addr = info.addresses().first();
    mySocket->connectToHost(addr,myPort);
    this->accept();
}

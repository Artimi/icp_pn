#include "netlist.h"
#include "ui_netlist.h"

NetList::NetList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetList)
{
    ui->setupUi(this);
}

NetList::~NetList()
{
    delete ui;
}

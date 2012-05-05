#include "userlogs.h"
#include "ui_userlogs.h"

UserLogs::UserLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogs)
{
    ui->setupUi(this);
}

UserLogs::~UserLogs()
{
    delete ui;
}

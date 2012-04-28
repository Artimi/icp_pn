#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow( QWidget *parent):
    QDialog(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);

}

LoginWindow::~LoginWindow()
{
    delete ui;
}

/**
  * @file about.cpp
  *
  * @brief Vyskakovací okno s informacemi o autorech
  * @author xsebek02, xsimon14
  */
#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

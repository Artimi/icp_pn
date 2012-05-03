/**
  * @file netinformation.cpp
  *
  * @brief Formulář pro zadávání informací o síti
  * @author xsebek02, xsimon14
  */
#include "netinformation.h"
#include "ui_netinformation.h"

NetInformation::NetInformation(DiagramScene  *scene, QString author, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::netinformation)
{
    ui->setupUi(this);
    myScene = scene;

    ui->LEAuthor->setText(author);
    ui->LEAuthor->setReadOnly(true);
    ui->LEName->setText(myScene->getName());
    ui->LEVersion->setText(myScene->getVersion());
    ui->LEVersion->setReadOnly(true);
    ui->TEDescription->setPlainText(myScene->getDescription());
    connect(ui->buttonBox,SIGNAL(accepted()),
                                 this, SLOT(saveInformation()));
}

NetInformation::~NetInformation()
{
    delete ui;
}

/**
  * Ukládá zapsané informace ve formuláři do scény
  */
void NetInformation::saveInformation()
{
    myScene->setAuthor(ui->LEAuthor->text());
    myScene->setName(ui->LEName->text());
    myScene->setVersion(ui->LEVersion->text());
    myScene->setDescription(ui->TEDescription->toPlainText());
}

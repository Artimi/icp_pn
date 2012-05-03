#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QSettings * settings,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    mySettings = settings;

    connect(ui->PBLineColor,SIGNAL(clicked()),this,SLOT(lineColorSettings()));
    connect(ui->PBDashLineColor,SIGNAL(clicked()),this,SLOT(dashLineColorSettings()));
    connect(ui->PBFont,SIGNAL(clicked()),this,SLOT(fontSettings()));
    connect(this,SIGNAL(accepted()),this,SLOT(saveSettings()));

    ui->SBObjectSize->setValue(mySettings->value("objectSize",100).toInt());

}

Settings::~Settings()
{
    delete ui;
}

void Settings::fontSettings()
{
    bool ok;
    myFont = QFontDialog::getFont(&ok,
                                  mySettings->value("font","Sans Serif,10,-1,5,50,0,0,0,0,0").value<QFont>(),
                                  this);
}

void Settings::lineColorSettings()
{
    myLineColor = QColorDialog::getColor(mySettings->value("lineColor","#000080").value<QColor>(),this);
}

void Settings::dashLineColorSettings()
{
    myDashLineColor = QColorDialog::getColor(mySettings->value("dashLineColor","#000000").value<QColor>(),this);
}

void Settings::saveSettings()
{
    mySettings->setValue("lineColor",myLineColor);
    mySettings->setValue("dashLineColor",myDashLineColor);
    mySettings->setValue("font",myFont);
    mySettings->setValue("objectSize",ui->SBObjectSize->value());
}

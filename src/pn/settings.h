/**
  * @file settings.h
  *
  * @author xsebek02, xsimon14
  */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QFontDialog>
#include <QColorDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
    
public:
    explicit Settings(QSettings *settings,QWidget *parent = 0);
    ~Settings();
    
public slots:
    void fontSettings();
    void lineColorSettings();
    void dashLineColorSettings();

    void saveSettings();

private:
    Ui::Settings *ui;

    QFont myFont;
    QColor myLineColor;
    QColor myDashLineColor;
    bool myMainWindowMaximized;

    QSettings *mySettings;

};

#endif // SETTINGS_H

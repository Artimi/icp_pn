/**
  * @file settings.h
  *
<<<<<<< HEAD
  * @author xsebek02, xsimon14
=======
  * @author xsebek02,xsimon14
>>>>>>> 16d51d74746a8cc1e8854c601db298d0ac7870a1
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

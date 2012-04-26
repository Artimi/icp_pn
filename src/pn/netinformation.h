/**
  * @file netinformation.h
  *
  * @author xsebek02, xsimon14
  */

#ifndef NETINFORMATION_H
#define NETINFORMATION_H

#include <QDialog>
#include <QtGui>
#include "diagramscene.h"

class QGraphicsScene;

namespace Ui {
class netinformation;
}

class NetInformation : public QDialog
{
    Q_OBJECT
    
public:
    explicit NetInformation(DiagramScene * scene ,QWidget *parent = 0);
    ~NetInformation();
    
private:
    Ui::netinformation *ui;
    DiagramScene * myScene;

private slots:
    void saveInformation();
};

#endif // NETINFORMATION_H

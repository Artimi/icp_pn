#ifndef NETLIST_H
#define NETLIST_H

#include <QDialog>
#include "diagramscene.h"


namespace Ui {
class NetList;
}

class NetList : public QDialog
{
    Q_OBJECT
    
public:
    explicit NetList(QList<DiagramScene *> *list,QWidget *parent = 0);
    ~NetList();
    
public slots:
    void updateTable();
    void find();

private:
    Ui::NetList *ui;
    QList<DiagramScene *> *myList;
};

#endif // NETLIST_H

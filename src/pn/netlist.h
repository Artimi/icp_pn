#ifndef NETLIST_H
#define NETLIST_H

#include <QDialog>

namespace Ui {
class NetList;
}

class NetList : public QDialog
{
    Q_OBJECT
    
public:
    explicit NetList(QWidget *parent = 0);
    ~NetList();
    
private:
    Ui::NetList *ui;
};

#endif // NETLIST_H

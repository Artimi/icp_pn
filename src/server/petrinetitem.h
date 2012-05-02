#ifndef PETRINETITEM_H
#define PETRINETITEM_H
#include <QtCore>

class PetriNetItem
{
public:
    enum {Type = 0};

    PetriNetItem();


    QString getName()
    {
        return name;
    }
    void setName(QString str)
    {name = str;}

    int type()
    {return Type;}

private:
    QString name;
};

#endif // PETRINETITEM_H

/**
  * @file petrinetitem.h
  *
  * @author xsebek02 xsimon14
  */
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

    virtual int type()
    {return Type;}

private:
    QString name;
};

#endif // PETRINETITEM_H

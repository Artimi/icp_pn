#ifndef PETRINETARROW_H
#define PETRINETARROW_H
#include <QtCore>
#include "petrinetitem.h"

class PetriNetArrow : public PetriNetItem
{
public:
    enum {Type = 1};

    PetriNetArrow(PetriNetItem *startItem, PetriNetItem *endItem);

    PetriNetItem *startItem()
    {return myStartItem;}

    PetriNetItem *endItem()
    {return myEndItem;}

    QString getVariable()
    {return variable;}

    bool setVariable(QString str);

    int type()
    {return Type;}

private:
    PetriNetItem * myStartItem;
    PetriNetItem * myEndItem;
    QString variable;



};

#endif // PETRINETARROW_H

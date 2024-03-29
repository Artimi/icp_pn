/**
  * @file petrinetitem.h
  *
  * @author xsebek02 xsimon14
  */
#ifndef PETRINETOBJECT_H
#define PETRINETOBJECT_H
#include <QtCore>
#include "petrinetitem.h"
#include "petrinetarrow.h"


class PetriNetObject : public PetriNetItem
{
public:
    PetriNetObject();

    qreal x()
    {return myX;}

    qreal y()
    {return myY;}

    void setX(qreal x)
    {myX = x;}
    void setY(qreal y)
    {myY = y;}

    void addArrow(PetriNetArrow *arrow);
    void addInArrow(PetriNetArrow *arrow);
    void addOutArrow(PetriNetArrow * arrow);

    QList<PetriNetArrow *> getInArrows()
    {return inArrows;}
    QList<PetriNetArrow *> getOutArrows()
    {return outArrows;}

protected:
    qreal myX, myY;
    QList<PetriNetArrow *> arrows;
    QList<PetriNetArrow *> inArrows;
    QList<PetriNetArrow *> outArrows;
};

#endif // PETRINETOBJECT_H

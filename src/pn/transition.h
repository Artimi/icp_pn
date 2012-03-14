/**
  * @file transition.cpp
  *
  * @author xsebek02, xsimon14
  */
#ifndef TRANSITION_H
#define TRANSITION_H


#include <QGraphicsItem>
#include <QString>
#include <QList>
#include <QPainter>
#include <QObject>
#include "diagramitem.h"

class QRectF;
class QPainter;
class QString;

class Transition : public DiagramItem
{

public:
    Transition(QGraphicsItem *parent =0);

    QRectF boundingRect() const
    {
        return rectangle;
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF rectangle;
};

#endif // TRANSITION_H

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
    enum {Type = UserType + 17};

    Transition(QGraphicsItem *parent =0);

    QRectF boundingRect() const
    {
        return rectangle;
    }

    int type() const
    {
        return Type;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString getGuard()
    {
        return guard;
    }

    void setGuard(QString str);

    QString getAction()
    {
        return action;
    }

    void setAction(QString str);

private:
    QRectF rectangle;

    static int count;

    QString guard;
    QString action;
};

#endif // TRANSITION_H

/**
  * @file transition.h
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
#include <QtGui>
#include <QMenu>
#include <QRegExp>
#include "diagramitem.h"

class QRectF;
class QPainter;
class QString;
class QMenu;
class QGraphicsSceneContextMenuEvent;

class Transition : public DiagramItem
{

public:
    enum {Type = UserType + 17};

    Transition(QMenu *menu = 0,QGraphicsItem *parent =0,QGraphicsScene * scene = 0);

    QRectF boundingRect() const
    {
        return rectangle.adjusted(-2,-2,2,2);
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

    bool setGuard(QString str);


    QString getAction()
    {
        return action;
    }

    bool setAction(QString str);

    bool chosen;

private:

    QRectF rectangle;
    QRectF guardRectangle;
    QRectF actionRectangle;

    int size;
    static int count;

    QString guard;
    QString action;

    QMenu *myMenu;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // TRANSITION_H

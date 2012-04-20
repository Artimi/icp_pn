#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <QtGui/QPen>
#include <QtGui/QPainter>

#include "diagramitem.h"
#include "place.h"

class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;

class Arrow : public QGraphicsLineItem
{
public:
    enum {Type = UserType +4};
    Arrow(DiagramItem *startItem, DiagramItem *endItem,
          QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);


    QRectF boundingRect() const;
    QPainterPath shape() const;

    DiagramItem *startItem() const
    {
        return myStartItem;
    }
    DiagramItem *endItem() const
    {
        return myEndItem;
    }

    void updatePosition();


protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QPolygonF arrowHead;
    QPointF sourcePoint;
    QPointF destPoint;

};

#endif // ARROW_H

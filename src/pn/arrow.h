#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <QtGui/QPen>
#include <QtGui/QPainter>

#include "diagramitem.h"
#include "place.h"
#include "transition.h"

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

    int type() const
    {return Type;}

    QString getVariable()
    {
        return variable;
    }

    void setVariable(QString str)
    {variable=str;}

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

    QPointF getIntersectionPoint(QLineF line,DiagramItem * item);

    QString variable;

};

#endif // ARROW_H

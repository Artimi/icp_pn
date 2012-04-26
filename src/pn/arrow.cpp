#include "arrow.h"
#include <math.h>

const qreal Pi = 3.14;

/**
  * Konstruktor, který vytvoří hranu
  * @param  startItem   item, ze kterého hrana vystupuje
  * @param  endItem     item, do kterého hrana vstupuje
  */
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem, QMenu *menu,
             QGraphicsItem *parent, QGraphicsScene *scene)
{
    myStartItem = startItem;
    myEndItem = endItem;
    variable = "";

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ));
    myMenu = menu;

    updatePosition();


}

/**
  * Vnější čtverec hrany
  */
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                     line().p2().y() - line().p1().y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

/**
  * Přesný tvar hrany
  */
QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}




/**
  * Přepočítá souřadnice počátku a konce hrany
  */
void Arrow::updatePosition()
{
    if (!myStartItem || !myEndItem)
        return;

    QPointF startCenter (myStartItem->boundingRect().height()/2,
                         myStartItem->boundingRect().width()/2);
    QPointF endCenter   (myEndItem->boundingRect().height()/2,
                         myEndItem->boundingRect().width()/2);

    QLineF line(myStartItem->pos() + startCenter,
                      myEndItem->pos() + endCenter);

    qreal length = line.length();
    prepareGeometryChange();

    if (length > qreal(70))
    {
        QPointF edgeOffset((line.dx() * 35) / length, (line.dy() * 35) / length);
        if (myStartItem->type() == Place::Type)
        {
            sourcePoint = line.p1() + edgeOffset;
            destPoint = getIntersectionPoint(line,myEndItem);

        }
        else
        {
            destPoint = line.p2() - edgeOffset;
            sourcePoint = getIntersectionPoint(line,myStartItem);
        }

    }
    else
    {
        sourcePoint = myStartItem->pos() + startCenter;
        destPoint = myEndItem->pos() + endCenter;
    }
}

/**
  * Hranu vykreslí
  */
void Arrow::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{

    setLine(QLineF(sourcePoint,destPoint));

    qreal arrowSize = 15;
    double angle = ::acos(line().dx() / line().length());

    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line().p2() + QPointF(-sin(angle + Pi / 3) * arrowSize,
                                    -cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p2() + QPointF(-sin(angle + Pi - Pi / 3) * arrowSize,
                                    -cos(angle + Pi - Pi / 3) * arrowSize);

    painter->drawLine(line());
    painter->drawLine(QLineF(destPoint, arrowP1));
    painter->drawLine(QLineF(destPoint, arrowP2));
    if (isSelected())
    {
        painter->setPen(QPen(Qt::red,1,Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}

QPointF Arrow::getIntersectionPoint(QLineF line, DiagramItem *item)
{
    QPolygonF transitionPolygon;
    QPointF p1, p2, result;
    QLineF polyLine;

    transitionPolygon = item->polygon();
    for(int i = 0; i < transitionPolygon.count(); ++i)
    {
        p2 = transitionPolygon.at(i) + item->pos();
        polyLine = QLineF(p1,p2);
        if (polyLine.intersect(line,&result) == QLineF::BoundedIntersection)
            break;

        p1 = p2;
    }
    return result;
}


void Arrow::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myMenu->exec(event->screenPos());
}

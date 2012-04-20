#include "arrow.h"
#include <math.h>

/**
  * Konstruktor, který vytvoří hranu
  * @param  startItem   item, ze kterého hrana vystupuje
  * @param  endItem     item, do kterého hrana vstupuje
  */
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem,
             QGraphicsItem *parent, QGraphicsScene *scene)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ));
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

    if (length > qreal(70)) {
        QPointF edgeOffset((line.dx() * 35) / length, (line.dy() * 35) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint =myStartItem->pos() + startCenter;
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

    painter->drawLine(QLineF(sourcePoint,destPoint));

}

/**
  * @file arrow.cpp
  *
  * @brief Zapouzdřuje položku grafu hrana
  * @author xsebek02, xsimon14
  */

#include "arrow.h"
#include "mainwindow.h"
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
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(20);
    path.moveTo(line().p1());
    path.lineTo(line().p2());
    return stroker.createStroke(path);
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

        if (myStartItem->type() == Place::Type)
        {
            QPointF edgeOffset((line.dx() * MainWindow::OBJECTSIZE/2) / length, (line.dy() * MainWindow::OBJECTSIZE/ 2) / length);
            sourcePoint = line.p1() + edgeOffset;
            destPoint = getIntersectionPoint(line,myEndItem);

        }
        else
        {
            QPointF edgeOffset((line.dx() * MainWindow::OBJECTSIZE/2) / length, (line.dy() *MainWindow::OBJECTSIZE / 2) / length);
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

bool Arrow::setVariable(QString str)
{

    QRegExp valid ("[a-zA-Z0-9]+");
    if(!valid.exactMatch(str))
        return false;

    variable=str;
    return true;
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

    QPointF center = (line().p1() + line().p2()) / 2;
    painter->setPen(QPen(MainWindow::LINECOLOR, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ));
    painter->drawLine(line());
    painter->drawLine(QLineF(destPoint, arrowP1));
    painter->drawLine(QLineF(destPoint, arrowP2));
    painter->setFont(MainWindow::FONT);
    painter->drawText(QRectF(center.x()-10,center.y() + 10, 50,50),variable);
    if (isSelected())
    {
        painter->setPen(QPen(MainWindow::DASHLINECOLOR,1,Qt::DashLine));
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

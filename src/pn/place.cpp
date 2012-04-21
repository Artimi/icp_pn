/**
  * @file place.cpp
  *
  * @brief Zapouzdřuje poližku grafu Místo
  * @author xsebek02, xsimon14
  */
#include "place.h"
#include <QMessageBox>

int Place::count = 0;

Place::Place(QGraphicsItem *parent) :
    DiagramItem(DiagramItem::Place,parent)
{
    size = 70;
    rectangle.setRect(0,0,size,size);
    myPolygon = QPolygonF(boundingRect());
    name.setNum(++count);
    name.prepend("p");
}

/**
  * metoda vykresluje objekt, reimplementovaná metoda
  *
  */
void Place::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isSelected())
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(rectangle);
    painter->drawText(rectangle,getTokenString());
}

/**
  * přidává do místa int token
  * @param token hodnota daného tokenu
  */
void Place::addToken(int token)
{
    tokens.append(token);
}

void Place::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}



/**
  * @return QString obsah tokens oddělený čárkami
  */
QString Place::getTokenString()
{
    QString result;
    QString s;
    foreach(s, tokens)
    {
        result += s + ", ";
    }
    return result;
}

void Place::setTokenString(QString str)
{

}

/**
  * Metoda pro zjištění přesného tvaru itemu
  * @return přesný tvar itemu
  */
QPainterPath Place::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

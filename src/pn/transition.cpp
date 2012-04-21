/**
  * @file transition.cpp
  *
  * @brief Zapouzdřuje poližku grafu Přechod
  * @author xsebek02, xsimon14
  */
#include "transition.h"

Transition::Transition(QGraphicsItem *parent) :
    DiagramItem(DiagramItem::Transition,parent)
{
    rectangle.setRect(0,0,70,70);
    rectangle.toRect();
    myPolygon = QPolygonF(boundingRect());
}

/**
  * metoda vykresluje objekt
  */
void Transition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isSelected())
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rectangle);
    painter->drawLine(0,rectangle.height()/2,rectangle.width(), rectangle.height()/2);
}

/**
  * @file transition.cpp
  *
  * @brief Zapouzdřuje poližku grafu Přechod
  * @author xsebek02, xsimon14
  */
#include "transition.h"

int Transition::count = 0;

Transition::Transition(QMenu *menu,QGraphicsItem *parent, QGraphicsScene *scene)
    :DiagramItem(DiagramItem::Transition,parent,scene)
{
    rectangle.setRect(0,0,70,70);
    rectangle.toRect();
    myPolygon = QPolygonF(boundingRect());
    name.setNum(++count);
    name.prepend("n");

    guard = "";
    action = "";
    myMenu = menu;
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


void Transition::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myMenu->exec(event->screenPos());
}

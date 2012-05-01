/**
  * @file transition.cpp
  *
  * @brief Zapouzdřuje položku grafu Přechod
  * @author xsebek02, xsimon14
  */
#include "transition.h"

int Transition::count = 0;

Transition::Transition(QMenu *menu,QGraphicsItem *parent, QGraphicsScene *scene)
    :DiagramItem(DiagramItem::Transition,parent,scene)
{
    size = 100;
    rectangle.setRect(0,0,size,size);
    rectangle.toRect();

    guardRectangle.setRect(2,2,size-2,size/2 -2);
    actionRectangle.setRect(2,size/2 + 2 ,size - 2,size -2 );
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
    {
        painter->setPen(QPen(Qt::darkBlue, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(boundingRect());
    }

    painter->setPen(Qt::black);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rectangle);
    painter->drawLine(0,rectangle.height()/2,rectangle.width(), rectangle.height()/2);
    painter->drawText(guardRectangle,getGuard());
    painter->drawText(actionRectangle,getAction());
}

/**
  * Nastavuje stráž přechodu
  *
  * @param  str vstupní řetězec se stráží
  * @return true -všechno v pořádku, false - nepodařilo se rozparsovat
  */
bool Transition::setGuard(QString str)
{
    QRegExp valid ("^(\\s*[a-zA-Z0-9]+\\s*(<|<=|>=|>|==|!=)\\s*[a-zA-Z0-9]+\\s*&)*\\s*[a-zA-Z0-9]+\\s*(<|<=|>=|>|==|!=)\\s*[a-zA-Z0-9]+\\s*$");
    if(!valid.exactMatch(str))
        return false;

    guard = str;
    return true;
}

/**
  * Nastavuje akci přechodu
  *
  * @param  str vstupní řetězec s akcí
  * @return true -  všechno  v pořádku, false - nepodařilo se rozparsovat
  */
bool Transition::setAction(QString str)
{
    QRegExp valid("[a-zA-Z]+\\s*=(\\s*[a-zA-Z0-9]+\\s*(\\+|\\-)\\s*)*\\s*[a-zA-Z0-9]+\\s*");
    if(!valid.exactMatch(str))
        return false;

    action = str;
    return true;
}

/**
  * Implementace vyskakování kontextového menu
  */
void Transition::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myMenu->exec(event->screenPos());
}

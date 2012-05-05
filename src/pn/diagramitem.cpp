/**
  * @file diagramitem.cpp
  *
  * @brief Třída obsahující položku diagramu: místo nebo přechod
  * @author xsebek02 xsimon14
  *
  */
#include "diagramitem.h"
#include "arrow.h"

/**
  * Konstruktor tridy DiagramItem
  */
DiagramItem::DiagramItem(DiagramType diagramType, QGraphicsItem *parent, QGraphicsScene *scene)
    :QGraphicsItem(parent,scene)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
}

/**
  * Vymaže ze seznamu napojených hran hranu arrow
  * @param  arrow   mazaná hrana
  */
void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

/**
  * Smaže všechny hrany napojené na item
  */
void DiagramItem::removeArrows()
{
    foreach(Arrow *arrow, arrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

/**
  * Připojí hranu do seznamu připojených hran
  * @param  arrow   připojovaná hrana
  */
void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

/**
  * Volán v případě změny pozice itemu, reimplementovaná metoda
  * @param  change  charakter změny itemu
  * @param  value   nová hodnota po změně (nepoužito)
  * @return value   nepoužito
  */
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach(Arrow *arrow,arrows)
        {
            arrow->updatePosition();
        }
    }
    return value;
}


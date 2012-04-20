/**
  * @file diagramitem.cpp
  *
  * @brief Třída obsahující položku diagramu: místo nebo přechod
  * @author xsebek02 xsimon14
  *
  */
#include "diagramitem.h"
#include "arrow.h"

DiagramItem::DiagramItem(DiagramType diagramType, QGraphicsItem *parent, QGraphicsScene *scene)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
}

void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach(Arrow *arrow, arrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        //scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}


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


/**
  * @file diagramscene.cpp
  *
  *
  * @brief Scena obsahujicí všechny grafické objekty
  * @author xsebek02 xsimon14
  *
  */

#include "diagramscene.h"

DiagramScene::DiagramScene()
{
    myMode = MoveItem;
    line = 0;
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

//TODO: editorLostFocus

/*void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
       return;

    DiagramItem *item;
    switch(myMode)
    {
    case InsertItem:
        if(myItemType == DiagramItem::Place)
        {
            item = new Place();
        }
        else if (myItemType == DiagramItem::Transition)
        {
            //TODO konstruktor Transition
        }
        addItem(item);
        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        break;

    case InsertLine:
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen(QPen(Qt::black));
        addItem(line);
        break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent); //?

}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(myMode == InsertLine && line != 0)
    {
//        QLineF newLine(line->line().pl(), mouseEvent->scenePos());
        //line->setLine(newLine);
    }
    else if (myMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

*/

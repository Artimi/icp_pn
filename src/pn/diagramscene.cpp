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

/**
  * Nastaví mod práce ve scene
  * @param mode mod práce InsertItem|InsertLine|MoveItem
  */
void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

/**
  * @return Mode současný mod scene
  */

DiagramScene::Mode DiagramScene::getMode()
{
    return myMode;
}

/**
  * Nastavuje typ vkládaného objektu
  * @param type typ vkládáného objektu DiagramItem::Place|DiagramItem::Transition
  */
void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

/**
  * @return DiagramType současný vkládaný objekt
  */
DiagramItem::DiagramType DiagramScene::getItemType()
{
    return myItemType;
}

//TODO: editorLostFocus

/**
  * Vyvoláno při zmáčknutí tlačítka myši, podle nastaveného modu a ItemType se
  * provede akce
  *
  * @param mouseEvent událost myši
  */
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
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
            item = new Transition();
        }
        addItem(item);
        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        break;

    case InsertLine:
/*        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen(QPen(Qt::black));
        addItem(line);
        */
        break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);

}

/**
  * Vyvoláno při pohnutí myší
  *
  * @param mouseEvent událost myši
  */

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



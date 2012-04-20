/**
  * @file diagramscene.cpp
  *
  *
  * @brief Scena obsahujicí všechny grafické objekty
  * @author xsebek02 xsimon14
  *
  */

#include "diagramscene.h"
#include "arrow.h"

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
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen(QPen(Qt::black));
        addItem(line);

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
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}


/**
  * Vyvoláno při uvolnění myši
  *
  * @param  mouseEvent  událost myši
  */
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine)
    {
            QList<QGraphicsItem *> startItems = items(line->line().p1());
            if (startItems.count() && startItems.first() == line)
                startItems.removeFirst();

            QList<QGraphicsItem *> endItems = items(line->line().p2());
            if (endItems.count() && endItems.first() == line)
                endItems.removeFirst();

            removeItem(line);
            delete line;

            if (startItems.count() > 0 && endItems.count() > 0 &&
                    startItems.first()->type() == DiagramItem::Type &&
                    endItems.first()->type() == DiagramItem::Type &&
                    startItems.first() != endItems.first())
            {
                DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
                DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *> (endItems.first());
                Arrow *arrow = new Arrow(startItem, endItem);
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                addItem(arrow);
                arrow->updatePosition();
            }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

}

/**
  * Smaže všechny označené itemy
  *
  */
void DiagramScene::deleteItem()
{
    foreach(QGraphicsItem *item, selectedItems())
    {
        if(item->type() == Arrow::Type)
        {
            removeItem(item);
            Arrow * arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach(QGraphicsItem *item, selectedItems())
    {
        if (item->type() == DiagramItem::Type)
        {
            qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
        }
        removeItem(item);
        delete item;
    }
}





















/**
  * @file diagramscene.cpp
  *
  * @brief Scena obsahujicí všechny grafické objekty
  * @author xsebek02 xsimon14
  *
  */

#include "diagramscene.h"
#include "mainwindow.h"

DiagramScene::DiagramScene(QMenu *placeMenu, QMenu *transitionMenu, QMenu *arrowMenu, QObject *parent)
    :   QGraphicsScene(parent)
{
    myMode = MoveItem;
    line = 0;
    version = "0";

    myPlaceMenu = placeMenu;
    myTransitionMenu = transitionMenu;
    myArrowMenu = arrowMenu;
}

DiagramScene::~DiagramScene()
{

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
            item = new Place(myPlaceMenu);
        }
        else if (myItemType == DiagramItem::Transition)
        {
            item = new Transition(myTransitionMenu);
        }
        else
        {
            break;
        }
        addItem(item);
        item->setPos(mouseEvent->scenePos());
        break;

    case InsertLine:
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
        line->setPen(QPen(MainWindow::LINECOLOR));
        addItem(line);
        break;
    case MoveItem:
        QGraphicsItem * itemUnderMouse = itemAt(mouseEvent->scenePos().x(),
                                                mouseEvent->scenePos().y());

        if(itemUnderMouse &&
           itemUnderMouse->isEnabled() &&
           itemUnderMouse->flags() & QGraphicsItem::ItemIsSelectable)
        {
            itemUnderMouse->setSelected(!itemUnderMouse->isSelected());
        }
        else if (!itemUnderMouse)
        {
            clearSelection();
        }
        break;


    }
    update();
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
    update();
}


/**
  * Vyvoláno při uvolnění myši
  *
  * @param  mouseEvent  událost myši
  */
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch(myMode)
    {
        case InsertLine:
            if (line != 0)
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
                            startItems.first() != endItems.first())
                    {
                        DiagramItem *startItem;
                        DiagramItem *endItem;

                        if(startItems.first()->type() == Place::Type &&
                                endItems.first()->type() == Transition::Type)
                        {
                            startItem = qgraphicsitem_cast<Place *>(startItems.first());
                            endItem = qgraphicsitem_cast<Transition *> (endItems.first());
                        }
                        else if(startItems.first()->type() == Transition::Type &&
                               endItems.first()->type() == Place::Type)
                        {
                            startItem = qgraphicsitem_cast<Transition *>(startItems.first());
                            endItem = qgraphicsitem_cast<Place *> (endItems.first());
                        }
                        else
                        {
                          break;
                        }

                        if (hasArc(startItem,endItem))
                                break;

                        Arrow *arrow = new Arrow(startItem, endItem, myArrowMenu);
                        startItem->addArrow(arrow);
                        endItem->addArrow(arrow);
                        addItem(arrow);
                        arrow->updatePosition();
                    }
            }
            line = 0;
    default: ;
    }
    update();

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

/**
  * Zjistí zda dané itemy mezi sebou mají hranu
  * @param  item1   dotazovaný objekt
  * @param  item2   dotazovaný objekt
  * @return true - mají mezi sebou hranu, false - nemají
  */
bool DiagramScene::hasArc(DiagramItem *item1, DiagramItem *item2)
{
    foreach(QGraphicsItem *item, items())
    {
        if (item->type() == Arrow::Type)
        {
            Arrow * arrow = qgraphicsitem_cast<Arrow *>(item);
            if( (arrow->startItem() == item1 &&
                arrow->endItem() == item2 ) ||
                (arrow->startItem() == item2 &&
                arrow->endItem() == item1 )     )
                return true;
        }
    }
    return false;
}

DiagramItem *DiagramScene::getDiagramItem(QString name)
{
    foreach(QGraphicsItem * item, items())
    {
        if(item->type() == Place::Type)
        {
            Place * place = qgraphicsitem_cast<Place *>(item);
            if(place->getName() == name)
                return place;
        }
        else if (item->type() == Transition::Type)
        {
            Transition * transition = qgraphicsitem_cast<Transition *>(item);
            if(transition->getName() == name)
                return transition;
        }
    }
    return NULL;
}



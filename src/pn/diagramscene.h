/**
  * @file diagramscene.h
  *
  * @author xsebek02 xsimon14
  *
  */

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QLineF>
#include "diagramitem.h"
#include "place.h"
#include "transition.h"


class QGraphicSceneMouseEvent;
class DiagramItem;
class QGraphicsItem;
class QPointF;
class QGraphicsLineItem;
class QLineF;



class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode {InsertItem, InsertLine, MoveItem};
    DiagramScene();

    Mode getMode();
    DiagramItem::DiagramType getItemType();

public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType type);

signals:
    void itemInserted(DiagramItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool isItemChange(int type);

    DiagramItem::DiagramType myItemType;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;

};

#endif // DIAGRAMSCENE_H

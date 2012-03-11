#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QtGui/QGraphicsScene>
#include "diagramitem.h"

class QGraphicSceneMouseEvent;



class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode {InsertItem, InsertLine, MoveItem};
    DiagramScene();

public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType type);

signals:
    void itemInserted(DiagramItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool isItemChange(int type);

    DiagramItem::DiagramType myItemType;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;

};

#endif // DIAGRAMSCENE_H


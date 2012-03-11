#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QtGui/QGraphicsPixmapItem>
#include <QList>


class DiagramItem : public QGraphicsPolygonItem, QGraphicsEllipseItem
{
public:
    enum DiagramType{Place, Transition};
    DiagramItem(DiagramType diagramType, QGraphicsItem *parent =0, QGraphicsScene *scene = 0);

    DiagramType diagramType()
    {return myDiagramType;}


private:
    DiagramType myDiagramType;
    //QAbstractGraphicsShapeItem myShape;
};

#endif // DIAGRAMITEM_H

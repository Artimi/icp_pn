/**
  * @file mainwindow.h
  *
  * @author xsebek02 xsimon14
  *
  */
#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QtGui/QGraphicsPixmapItem>
#include <QList>
#include <QObject>

class DiagramItem : public QGraphicsItem
{

public:
    enum DiagramType{Place, Transition};
    DiagramItem(DiagramType diagramType, QGraphicsItem *parent =0, QGraphicsScene *scene = 0);

    DiagramType diagramType()
    {return myDiagramType;}
    QPolygonF polygon() const
        { return myPolygon; }

private:
    DiagramType myDiagramType;
    QPolygonF myPolygon;
};

#endif // DIAGRAMITEM_H


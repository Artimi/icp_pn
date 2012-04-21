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

class Arrow;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneHoverEvent;


class DiagramItem : public QGraphicsItem
{

public:
    enum {Type = UserType + 15};
    enum DiagramType{Place, Transition};
    DiagramItem(DiagramType diagramType, QGraphicsItem *parent =0,
                QGraphicsScene *scene = 0);

    DiagramType diagramType()
    {return myDiagramType;}
    QPolygonF polygon() const
        { return myPolygon; }

    int type() const
    {return Type;}

    QString getName()
    {
        return name;
    }
    void setName(QString str)
    {name = str;}

    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QList<Arrow *> arrows;
    QString name;

    QMenu *myContextMenu;



};

#endif // DIAGRAMITEM_H


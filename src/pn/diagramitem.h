/**
  * @file diagramitem.h
  *
  * @author xsebek02 xsimon14
  *
  */
#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QtGui/QGraphicsPixmapItem>
#include <QList>
#include <QObject>
#include <QColor>
#include <QFont>
class Arrow;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneHoverEvent;


class DiagramItem : public QGraphicsItem
{

public:
    enum {Type = UserType + 15};
    enum DiagramType{Place, Transition};
    DiagramItem(DiagramType diagramType,
                QGraphicsItem *parent =0,QGraphicsScene *scene = 0);

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

    void setLineColor(QColor *lineColor)
    {myLineColor = lineColor;}
    void setDashLineColor(QColor *dashLineColor)
    {myDashLineColor = dashLineColor;}
    void setFontSize(int * fontSize)
    {myFontSize = fontSize;}
    void setObjectSize(int * objectSize)
    {myObjectSize = objectSize;}


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QList<Arrow *> arrows;
    QString name;

    QColor *myLineColor;
    QColor *myDashLineColor;
    int *myFontSize;
    int *myObjectSize;



};

#endif // DIAGRAMITEM_H


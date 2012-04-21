/**
  * @file place.h
  *
  * @author xsebek02,xsimon14
  */
#ifndef PLACE_H
#define PLACE_H

#include <QGraphicsItem>
#include <QString>
#include <QList>
#include <QPainter>
#include <QObject>
#include <QtGui>
#include "diagramitem.h"

class QRectF;
class QPainter;
class QString;
class QMenu;
class QGraphicsSceneContextMenuEvent;


class Place : public DiagramItem
{

public:
    enum {Type = UserType + 16};
    Place(QGraphicsItem *parent = 0);
    int type() const
    {return Type;}

    QRectF boundingRect() const
    {
        return rectangle;
    }

    QPainterPath shape() const;


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addToken(int token);

    int size;

    static int count;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    QString getTokenString();
    void setTokenString(QString str);

    QList<int> getTokens()
    {
        return tokens;
    }

private:
    QRectF rectangle;
    QList<int> tokens;

    
};

#endif // PLACE_H

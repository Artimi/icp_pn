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
#include <QMenu>
#include "diagramitem.h"
#include <QRegExp>

class MainWindow;

class QRectF;
class QPainter;
class QString;
class QMenu;
class QGraphicsSceneContextMenuEvent;


class Place : public DiagramItem
{

public:
    enum {Type = UserType + 16};
    Place(QMenu *menu = 0,QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const
    {return Type;}

    QRectF boundingRect() const
    {
        return rectangle.adjusted(-2,-2,2,2);
    }

    QPainterPath shape() const;


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addToken(int token);

    int size;

    static int count;


    QString getTokenString();
    bool setTokenString(QString str);

    QList<int> getTokens()
    {
        return tokens;
    }

private:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    QRectF rectangle;
    QRectF textRectangle;
    QList<int> tokens;

    QMenu *myMenu;
    
};

#endif // PLACE_H

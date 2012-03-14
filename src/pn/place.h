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
#include "diagramitem.h"

class QRectF;
class QPainter;
class QString;


class Place : public DiagramItem
{

public:
    Place(QGraphicsItem *parent = 0);

    QRectF boundingRect() const
    {
        return rectangle;
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addToken(int token);

signals:
    
public slots:

private:
    QRectF rectangle;
    QList<int> tokens;

    QString getTokenString();

    
};

#endif // PLACE_H

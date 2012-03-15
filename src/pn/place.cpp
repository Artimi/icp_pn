/**
  * @file place.cpp
  *
  * @brief Zapouzdřuje poližku grafu Místo
  * @author xsebek02, xsimon14
  */
#include "place.h"

Place::Place(QGraphicsItem *parent) :
    DiagramItem(DiagramItem::Place,parent)
{
    rectangle.setRect(0,0,50,50);
}

/**
  * metoda vykresluje objekt
  *
  */
void Place::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(rectangle);
    painter->drawText(rectangle,getTokenString());
}

/**
  * přidává do místa int token
  * @param token hodnota daného tokenu
  */
void Place::addToken(int token)
{
    tokens.append(token);
}

/**
  * @return QString obsah tokens oddělený čárkami
  */
QString Place::getTokenString()
{
    QString result;
    QString s;
    foreach(s, tokens)
    {
        result += s + ", ";
    }
    return result;
}

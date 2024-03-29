/**
  * @file place.cpp
  *
  * @brief Zapouzdřuje položku grafu Místo
  * @author xsebek02, xsimon14
  */
#include "place.h"
#include <QMessageBox>
#include "mainwindow.h"


Place::Place(QMenu *menu, QGraphicsItem *parent, QGraphicsScene *scene) :
    DiagramItem(DiagramItem::Place,parent, scene)
{
    size = MainWindow::OBJECTSIZE;
    rectangle.setRect(0,0,size,size);
    textRectangle.setRect(size/4,size/4,3*size/4,3*size/4);
    myPolygon = QPolygonF(boundingRect());
    myMenu = menu;

    if (name.isEmpty())
    {
        DiagramScene * rootScene = (DiagramScene *) this->scene();
        rootScene->incPlaceCount();
        name = "p" + QString::number(rootScene->getPlaceCount());
    }
}

/**
  * metoda vykresluje objekt, reimplementovaná metoda
  *
  */
void Place::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    size = MainWindow::OBJECTSIZE;
    rectangle.setRect(0,0,size,size);
    textRectangle.setRect(size/4,size/4,3*size/4,3*size/4);
    if(isSelected())
    {
        painter->setPen(QPen(MainWindow::DASHLINECOLOR, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(boundingRect());
    }

    painter->setPen(MainWindow::LINECOLOR);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(rectangle);

    painter->setFont(MainWindow::FONT);
    painter->drawText(textRectangle,getTokenString());
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
    int tokensCount = tokens.count();
    int i = 0;
    QList<int>::const_iterator iter = tokens.begin();
    QList<int>::const_iterator end = tokens.end();
    for(; iter != end; iter++)
    {
        s = QString::number((*iter));
        if (i != tokensCount - 1)
            result += s + ", ";
        else
            result += s;
        i++;
    }
    return result;
}

/**
  * Z řetězce rozparsuje tokeny a vloží je místa
  *
  * @param  str vstupní řetězec s tokeny
  * @return true - všechno ok, false - nepodařilo se rozparsovat
  */
bool Place::setTokenString(QString str)
{
    if (str.isEmpty())
    {
        tokens.clear();
        return true;
    }
     QRegExp valid ("[-\\d\\s,]*");
    if(!valid.exactMatch(str))
        return false;

    tokens.clear();

    QRegExp rx("(-?\\d+)");
    int pos = 0;

    while ((pos = rx.indexIn(str,pos)) != -1)
    {
        addToken(rx.cap(1).toInt());
        pos += rx.matchedLength();
    }
    return true;
}

/**
  * Procedura implementující vyskakkování kontextového menu
  */
void Place::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myMenu->exec(event->screenPos());
}

/**
  * Metoda pro zjištění přesného tvaru itemu
  * @return přesný tvar itemu
  */
QPainterPath Place::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

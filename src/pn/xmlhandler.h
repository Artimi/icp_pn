/**
  * @file xmlhandler.h
  *
  * @author xsebek02, xsimon14
  */
#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QGraphicsItem>
#include <QFileDialog>
#include <QFile>
#include <QObject>
#include <QDebug>
#include "transition.h"
#include "place.h"
#include "arrow.h"
#include "diagramitem.h"
#include "diagramscene.h"

class XMLHandler
{
public:
    XMLHandler(DiagramScene * scene, QString name ="");
    QString toStr();
    void saveToFile(QFile *file);

    int  loadFromFile(QFile *file);
private:
    DiagramScene *myScene;
    QString myName;

    void writePetriNet(QXmlStreamWriter *writer);
    void writePlace(QXmlStreamWriter * writer, Place * place);
    void writeTransition(QXmlStreamWriter * writer, Transition * transition);
    void writeArc(QXmlStreamWriter * writer, Arrow * arrow);

    int readPetriNet(QXmlStreamReader *reader);
    int readPlace(QXmlStreamReader * reader);
    int readTransition(QXmlStreamReader * reader);
    int readArc(QXmlStreamReader * reader);
};

#endif // XMLHANDLER_H

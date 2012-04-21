#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QGraphicsItem>
#include <QFileDialog>
#include <QFile>
#include <QObject>
#include "transition.h"
#include "place.h"
#include "arrow.h"
#include "diagramitem.h"
#include "diagramscene.h"

class XMLHandler
{
public:
    XMLHandler(DiagramScene * scene, QString name ="");
    QString toXML();
    void saveToXMLFile(QFile *file);
private:
    DiagramScene *myScene;
    QString myName;

    void writePetriNet(QXmlStreamWriter *writer);
    void writePlace(QXmlStreamWriter * writer, Place * place);
    void writeTransition(QXmlStreamWriter * writer, Transition * transition);
    void writeArc(QXmlStreamWriter * writer, Arrow * arrow);
};

#endif // XMLHANDLER_H

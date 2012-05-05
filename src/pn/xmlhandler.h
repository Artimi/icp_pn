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
#include "message.h"

class XMLHandler
{
public:
    XMLHandler();

    QString toStr();
    void saveNetToFile(QFile *file);

    int  loadNetFromFile(QFile *file);

    QString writeMessage();
    int readMessage(QString str);

    void setScene(DiagramScene * scene)
    {myScene = scene;}
    void setMessage(Message * message)
    {myMessage = message;}
    void setNetList(QList<DiagramScene *> * netList)
    {myNetList = netList;}

    void setUserLogsList(QList< QList<QString> > * userLogsList)
    {myUsersLogsList = userLogsList;}


    Message * getMessage()
    {return myMessage;}
    DiagramScene * getScene()
    {return myScene;}
    QList<DiagramScene *> * getNetList()
    {return myNetList;}

    QList< QList<QString> > * getUserLogsList()
    {return myUsersLogsList;}


private:
    DiagramScene *myScene;
    Message *myMessage;
    QList<DiagramScene *> *myNetList;

    QList< QList<QString> > *myUsersLogsList;


    void writePetriNetInformation(QXmlStreamWriter * writer);
    void writePetriNet(QXmlStreamWriter *writer);
    void writePlace(QXmlStreamWriter * writer, Place * place);
    void writeTransition(QXmlStreamWriter * writer, Transition * transition);
    void writeArc(QXmlStreamWriter * writer, Arrow * arrow);


    int readPetriNetList(QXmlStreamReader *reader);
    int readPetriNet(QXmlStreamReader *reader);
    int readPlace(QXmlStreamReader * reader);
    int readTransition(QXmlStreamReader * reader);
    int readArc(QXmlStreamReader * reader);
    int readUserLogs(QXmlStreamReader * reader);


};

#endif // XMLHANDLER_H

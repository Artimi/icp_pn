#ifndef XMLHANDLERSERV_H
#define XMLHANDLERSERV_H

#include <QtCore>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>

#include "../pn/message.h"
#include "petrinet.h"
#include "petrinetitem.h"
/**
  * @file xmlhandler.h
  *
  * @author xsebek02 xsimon14
  */
#include "petrinetobject.h"
#include "petrinetarrow.h"
#include "petrinetplace.h"
#include "petrinettransition.h"

class XMLHandler
{
public:
    XMLHandler();

    QString toStr();

    void saveNetToFile(QFile * file);
    int loadNetFromFile(QFile *file);

    QString writeMessage();
    int readMessage(QString str);

    void setPetriNet(PetriNet * petriNet)
    {myPetriNet = petriNet;}
    void setMessage(Message * message)
    {myMessage = message;}
    void setNetList(QList<PetriNet *> * netList)
    {myNetList = netList;}

    PetriNet * getPetriNet()
    {return myPetriNet;}
    Message * getMessage()
    {return myMessage;}
    QList<PetriNet *> * getNetList()
    {return myNetList;}



private:
    PetriNet *myPetriNet;
    Message * myMessage;
    QList<PetriNet *> *myNetList;

    void writePetriNetInformation(QXmlStreamWriter * writer);
    void writePetriNetList(QXmlStreamWriter *writer);
    void writePetriNet(QXmlStreamWriter *writer);
    void writePlace(QXmlStreamWriter * writer, PetriNetPlace * place);
    void writeTransition(QXmlStreamWriter * writer, PetriNetTransition * transition);
    void writeArc(QXmlStreamWriter * writer, PetriNetArrow * arrow);
    void writeLog(QXmlStreamWriter *writer, QString user);

    int readPetriNet(QXmlStreamReader *reader);
    int readPlace(QXmlStreamReader * reader);
    int readTransition(QXmlStreamReader * reader);
    int readArc(QXmlStreamReader * reader);



};

#endif // XMLHANDLERSERV_H

/**
  * @file manipulatenet.h
  *
  * @author xsebek02, xsimon14
  */
#ifndef MANIPULATENET_H
#define MANIPULATENET_H
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include "xmlhandler.h"
#include "petrinet.h"

class ManipulateNet
{
public:
    ManipulateNet();
    bool error;
    QString apppath;
    void saveNet(QString name, QString username, XMLHandler *xmlhandler);
    void loadNet(QString name, QString username, QString version, PetriNet *resultNet);
    void getNetList(QString username, QList<PetriNet *> result);
private:

    int investigateVersion(QString name, QString username);
};

#endif // MANIPULATENET_H

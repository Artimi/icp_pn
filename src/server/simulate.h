/**
  * @file simulate.h
  *
  * @author xsebek02, xsimon14
  */
#ifndef SIMULATE_H
#define SIMULATE_H
#include <QString>
#include "xmlhandler.h"
//#include <QXmlStreamReader>
//#include <QXmlStreamWriter>
//#include "manipulatenet.h"
#include <QRegExp>
#include "petrinet.h"


class Simulate
{
public:
    Simulate();

    bool error;

    void SimulateStep(PetriNet * petriNet);
    void SimulateAll(PetriNet * petriNet);
    void getPairs(QList<PetriNetArrow *> inArrows,QString guard, QMap<Arrow*,int>* pairs);
private:

};

#endif // SIMULATE_H

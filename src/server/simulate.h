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
    void getPairs(QList<PetriNetArrow *> inArrows,QString guard, QMap<PetriNetArrow *, int> *pairs);


private:
    bool checkCondition(QString oper, int op1, int op2);
    bool transitionGuard(QMap<PetriNetArrow *,int> * map,QString guard);

    int getFactor(QMap<PetriNetArrow *,int> *map, count);

};

#endif // SIMULATE_H

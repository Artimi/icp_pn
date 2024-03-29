/**
  * @file simulate.h
  *
  * @author xsebek02, xsimon14
  */
#ifndef SIMULATE_H
#define SIMULATE_H
#include <QString>
#include "xmlhandler.h"
#include <QRegExp>
#include "petrinet.h"


class Simulate
{
public:
    Simulate();

    bool error;

    bool simulateStep(PetriNet * petriNet);
    bool simulateAll(PetriNet * petriNet);
    bool getPairs(QList<PetriNetArrow *> inArrows,QString guard, QMap<PetriNetArrow *, int> *pairs);


private:
    bool checkCondition(QString oper, int op1, int op2);
    bool transitionGuard(QMap<PetriNetArrow *,int> * map,QString guard);

    void transitionAction(QMap<PetriNetArrow *,int> * input,QMap<PetriNetArrow *, int> *output, QString actionGot);

    int getFactor(QMap<PetriNetArrow *,int> *map, int count);
    void removeTokens(QMap<PetriNetArrow *,int> *map);

};

#endif // SIMULATE_H

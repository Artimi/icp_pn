#ifndef PETRINETTRANSITION_H
#define PETRINETTRANSITION_H


#include <QtCore>
#include "petrinetitem.h"

class PetriNetTransition : public PetriNetItem
{
public:
    enum {Type = 3};

    PetriNetTransition();

    QString getGuard()
    {return guard;}

    bool setGuard(QString str);

    QString getAction()
    {return action;}

    bool setAction(QString str);


private:
    QString guard;
    QString action;
};

#endif // PETRINETTRANSITION_H

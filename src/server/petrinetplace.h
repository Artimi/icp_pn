#ifndef PETRINETPLACE_H
#define PETRINETPLACE_H

#include <QtCore>
#include "petrinetobject.h"

class PetriNetPlace : public PetriNetObject
{
public:
    enum {Type = 2};

    PetriNetPlace();

    void addToken(int token);

    QList<int> getTokens()
    {return tokens;}


private:
    QList<int> tokens;
};

#endif // PETRINETPLACE_H

#ifndef PETRINETPLACE_H
#define PETRINETPLACE_H

#include <QtCore>
#include "petrinetitem.h"

class PetriNetPlace : public PetriNetItem
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

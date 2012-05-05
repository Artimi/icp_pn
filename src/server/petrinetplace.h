/**
  * @file petrinetitem.h
  *
  * @author xsebek02 xsimon14
  */
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

    bool removeToken(int token);

    int type()
    {return Type;}
private:
    QList<int> tokens;
};

#endif // PETRINETPLACE_H

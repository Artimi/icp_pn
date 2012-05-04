#include "petrinettransition.h"


PetriNetTransition::PetriNetTransition()
{
    chosen = false;
}

bool PetriNetTransition::setGuard(QString str)
{
    QRegExp valid ("^(\\s*[a-zA-Z0-9]+\\s*(<|<=|>=|>|==|!=)\\s*[a-zA-Z0-9]+\\s*&)*\\s*[a-zA-Z0-9]+\\s*(<|<=|>=|>|==|!=)\\s*[a-zA-Z0-9]+\\s*$");
    if(!valid.exactMatch(str))
        return false;

    guard = str;
    return true;
}

bool PetriNetTransition::setAction(QString str)
{
    QRegExp valid("([a-zA-Z]+\\s*=(\\s*[a-zA-Z0-9]+\\s*(\\+|\\-)\\s*)*\\s*[a-zA-Z0-9]+\\s*,)*\\s*[a-zA-Z]+\\s*=(\\s*[a-zA-Z0-9]+\\s*(\\+|\\-)\\s*)*\\s*[a-zA-Z0-9]+\\s*");
    if(!valid.exactMatch(str))
        return false;

    action = str;
    return true;
}

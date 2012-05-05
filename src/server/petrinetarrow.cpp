/**
  * @file petrinetarrow.cpp
  *
  * @brief Implementace hrany petriho sítě
  * @author xsebek02 xsimon14
  */
#include "petrinetarrow.h"

PetriNetArrow::PetriNetArrow(PetriNetItem *startItem, PetriNetItem *endItem)
{
    myStartItem = startItem;
    myEndItem = endItem;
}

bool PetriNetArrow::setVariable(QString str)
{

    QRegExp valid ("[a-zA-Z0-9]+");
    if(!valid.exactMatch(str))
        return false;

    variable=str;
    return true;
}

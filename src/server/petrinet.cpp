/**
  * @file petrinet.cpp
  *
  * @brief Implementace petriho sítě
  * @author xsebek02 xsimon14
  */
#include "petrinet.h"

PetriNet::PetriNet()
{
}

PetriNet::~PetriNet()
{
    foreach(PetriNetItem * item, myItems)
    {
        delete item;
    }
}

void PetriNet::addItem(PetriNetItem *item)
{
    myItems.append(item);
}

PetriNetItem *PetriNet::getPetriNetItem(QString name)
{
    foreach(PetriNetItem * item, items())
    {
        if(item->type() == PetriNetPlace::Type)
        {
            PetriNetPlace * place = (PetriNetPlace *) item;
            if(place->getName() == name)
                return place;
        }
        else if(item->type() == PetriNetTransition::Type)
        {
            PetriNetTransition * transition = (PetriNetTransition *) item;
            if(transition->getName() == name)
                return transition;
        }
    }
    return NULL;
}

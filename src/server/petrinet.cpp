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

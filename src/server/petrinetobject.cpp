#include "petrinetobject.h"

PetriNetObject::PetriNetObject()
{
}

void PetriNetObject::addArrow(PetriNetArrow *arrow)
{
    arrows.append(arrow);
}

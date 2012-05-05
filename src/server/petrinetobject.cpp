/**
  * @file petrinetobject.cpp
  *
  * @brief Rodičovská třída místa a přechodu
  * @author xsebek02 xsimon14
  */
#include "petrinetobject.h"

PetriNetObject::PetriNetObject()
{
}

void PetriNetObject::addArrow(PetriNetArrow *arrow)
{
    arrows.append(arrow);
}

void PetriNetObject::addInArrow(PetriNetArrow *arrow)
{
    inArrows.append(arrow);
}

void PetriNetObject::addOutArrow(PetriNetArrow *arrow)
{
    outArrows.append(arrow);
}

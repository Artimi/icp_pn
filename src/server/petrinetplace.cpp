/**
  * @file petrinetplace.cpp
  *
  * @brief Místo petriho sítě
  * @author xsebek02 xsimon14
  */
#include "petrinetplace.h"

PetriNetPlace::PetriNetPlace()
{
}

void PetriNetPlace::addToken(int token)
{
    tokens.append(token);
}

bool PetriNetPlace::removeToken(int token)
{
    if (tokens.contains(token))
    {
        tokens.removeOne(token);
        return true;
    }
    return false;
}

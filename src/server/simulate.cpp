/**
  * @file simulate.h
  * @brief Trida pro simulaci Petriho siti
  * @author xsebek02, xsimon14
  */
#include "simulate.h"

/**
  * Konstruktor tridy pro simulaci
  */
Simulate::Simulate()
{
    this->error = false;
}


/**
  * Odsimuluje sit az do konce
  */
void Simulate::SimulateAll(PetriNet *petriNet)
{
    qDebug() << "Cela simulace";
    petriNet->setAuthor("Uzivatel po simulaci");
    petriNet->setDescription("The end state of simulation");

}


/**
  * Odsimuluje jeden krok podle zvoleneho prechodu
  */
void Simulate::SimulateStep(PetriNet *petriNet)
{
    qDebug() << "Jeden krok";
}

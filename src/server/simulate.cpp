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
    PetriNetTransition * transition;
    QList<PetriNetArrow *> inArrows;
    QList<PetriNetArrow *> outArrows;
    QList<PetriNetItem *> netItemList = petriNet->items();
    for (int i = 0; i < netItemList.size(); i++)
    {
        if (netItemList.at(i)->type() == PetriNetTransition::Type)
        {
            /* Item je prechod */
            transition = (PetriNetTransition *) netItemList.at(i);
            if (transition->chosen)
            {
                /* Prechod, ktery me zajima */
                inArrows = transition->getInArrows();
                outArrows = transition->getOutArrows();
                QMap<Arrow*,int> * pairs;
                getPairs(inArrows,transition->getGuard(),pairs);
                if(!this->error)
                {
                    /* Vsecko probehlo v poradku, navazano je, pokracuju */
                }
                else
                {
                    /* Nepodarilo se navaz, vracim chybu */

                }


                break;
            }
        }
    }
    //petriNet->setAuthor("Uzivatel po simulaci");
    //petriNet->setDescription("The end state of simulation");

}

/**
  * Vrati QMap uspesneho prechodu
  */
void Simulate::getPairs(QList<PetriNetArrow *> inArrows,QString guard, QMap<PetriNetArrow*,int>* pairs)
{
    //tady by mohla byt nejaka kontrola
    QMap<Arrow*,int> workingPairs;
    for(int i = 0; i < inArrows; i++)
    {
        /* Projdu vsechny vstupni hrany */
    }


}




/**
  * Odsimuluje jeden krok podle zvoleneho prechodu
  */
void Simulate::SimulateStep(PetriNet *petriNet)
{
    qDebug() << "Jeden krok";
}


bool Simulate::checkCondition(QString oper, int op1, int op2)
{
//    <, <=, >=, >, ==, !=
    if (oper == "<")
        return op1 < op2;
    else if (oper == "<=")
        return op1 <= op2;
    else if (oper == ">=")
        return op1 >= op2;
    else if (oper == ">")
        return op1 > op2;
    else if (oper == "==")
        return op1 == op2;
    else if (oper == "!=")
        return op1 != op2;
    else
        return false; // nějaká chyba

}


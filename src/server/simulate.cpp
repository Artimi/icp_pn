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
    int itemStates = getFactor(pairs,pairs->count);

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

bool Simulate::transitionGuard(QMap<PetriNetArrow *, int> *map, QString guard)
{

}





int Simulate::getFactor(QMap<PetriNetArrow *, int> *map, count)
{
    int factor = 1;
    int i = 0;
    int tokenCount;
    QMap<PetriNetArrow*,int>::const_iterator iter = map->begin();
    while((iter != map->end()) || i < count)
    {
        PetriNetPlace *place = (PetriNetPlace *) iter.key()->startItem();
        tokenCount = place->getTokens().count();

        if(tokenCount == 0)
            tokenCount = 1; //?? nesmím násobit nulou nebo by to bylo špatně, je jednička dobrý nápad?

        factor *= tokenCount;
        i++;
    }
    return factor;
}


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
void Simulate::SimulateStep(PetriNet *petriNet)
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
                QMap<PetriNetArrow*,int> * pairs;
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
    pairs= new QMap<PetriNetArrow*,int>;
    int arc;

    //naplnění pairs key value
    for(arc = 0; arc < inArrows.count(); arc++)
    {
        (*pairs)[inArrows[arc]]=0;
    }

    int itemStates = getFactor(pairs,pairs->count());


    for (int i = 0 ; i < itemStates; i++)
    {
        arc = 0;
        for(arc = 0; arc < inArrows.count(); arc++)
        {
            PetriNetPlace * place = (PetriNetPlace *) inArrows[arc]->startItem();

            (*pairs)[inArrows[arc]] = place->getTokens().at((i/getFactor(pairs,arc))%place->getTokens().count());

            arc++;
        }

        if(transitionGuard(pairs,guard))
            break; //nasel jsem v pairs je sprváná kombinace tokenů
    }
    if (i == itemStates)
        error = true; //pokud jsem prošel všechny stavy a žádný nevyhovoval je chyba, nemám co navázat
    else
        return;
}




/**
  * Odsimuluje jeden krok podle zvoleneho prechodu
  */
void Simulate::SimulateAll(PetriNet *petriNet)
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
    {
        error = true;
        return false; // nějaká chyba
    }
}

bool Simulate::transitionGuard(QMap<PetriNetArrow *, int> *map, QString guard)
{
    QRegExp rx("^([^//s])//s*([^//s])//s*([^//s])$");
    if(rx.indexIn(guard) != -1)
    {
        /* Odpovida predpisu */
        QString op1Var,op2Var,oper;
        oper = rx.cap(2);
        op1Var = rx.cap(1);
        op2Var = rx.cap(3);
        int op1,op2;
        QList<PetriNetArrow *> keys = map->keys();
        for (int i = 0; i < keys.size(); i++)
        {
            /* Projdu vsechny prvky */
            if (keys.at(i)->getVariable() == op1Var)
            {
                /* Jsem na indexu, kde odpovida promena 1, takze vytahnu hodnotu */
                op1 = map->value(keys.at(i));
            }

            if (keys.at(i)->getVariable() == op2Var)
            {
                /* Jsem na indexu, kde odpovida promena 2, takze vytahnu hodnotu */
                op2 = map->value(keys.at(i));
            }
        }
        return checkCondition(oper,op1,op2);
    }
    else
    {
        /* Neodpovida predpisu */
        return false;
    }
}





int Simulate::getFactor(QMap<PetriNetArrow *, int> *map,int count)
{
    int factor = 1;
    int i = 0;
    int tokenCount;
    QMap<PetriNetArrow*,int>::const_iterator iter = map->begin();
    while((iter != map->end()) && i < count)
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


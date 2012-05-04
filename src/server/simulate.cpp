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
  * Odsimuluje jeden krok simulace
  */
bool Simulate::SimulateStep(PetriNet *petriNet)
{
    qDebug() << "Jeden krok simulace";
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
                QMap<PetriNetArrow*,int>  pairs;
                QMap<PetriNetArrow *,int>  output;
                getPairs(inArrows,transition->getGuard(),&pairs);
                if(!this->error)
                {
                    /* Vsecko probehlo v poradku, navazano je, pokracuju */
                    for(int arc = 0; arc < outArrows.count(); arc++)
                    {
                        output[outArrows[arc]]=0; //garbage hodnota
                    }

                    transitionAction(&pairs,&output,transition->getAction());
                    if(!error)
                    {
                        removeTokens(&pairs);
                    }
                    else
                        return false;
                }
                else
                {
                    /* Nepodarilo se navaz, vracim chybu */
                    return false;
                }


                break;
            }
        }
    }
    return true;
}

/**
  * Vrati QMap uspesneho prechodu
  */
void Simulate::getPairs(QList<PetriNetArrow *> inArrows,QString guard, QMap<PetriNetArrow*,int>* pairs)
{
    int arc;
    int i;
    int factor;

    //naplnění pairs key value
    for(arc = 0; arc < inArrows.count(); arc++)
    {
        (*pairs)[inArrows[arc]]=0;
    }

    int itemStates = getFactor(pairs,pairs->count());

    for (i = 0 ; i < itemStates; i++)
    {
        arc = 0;
        for(arc = 0; arc < inArrows.count(); arc++)
        {
            PetriNetPlace * place = (PetriNetPlace *) inArrows[arc]->startItem();
            factor = getFactor(pairs,arc);

            if(factor == 0) // pokud nemá žádný token, smažu ho ze vstupních, stejně tam nemá co přijít
                pairs->remove(inArrows[arc]);
            else
                (*pairs)[inArrows[arc]] = place->getTokens().at((i/getFactor(pairs,arc))%place->getTokens().count());
        }

        if(transitionGuard(pairs,guard))
        {
            break; //nasel jsem v pairs je správná kombinace tokenů
        }

    }
    if (i == itemStates)
        error = true; //pokud jsem prošel všechny stavy a žádný nevyhovoval je chyba, nemám co navázat
    else
        return;
}




/**
  * Odsimuluje celou petriho síť
  */
bool Simulate::SimulateAll(PetriNet *petriNet)
{
    qDebug() << "Cela simulace";

    int stepsDone = 0;
    int maxSteps = 20;
    QList<PetriNetItem *> netItemList = petriNet->items();
    PetriNetTransition * transition;

    while(stepsDone < maxSteps && !error)
    {
        for(int i = 0; i < netItemList.count(); i++)
        {
            if(netItemList.at(i)->type() == PetriNetTransition::Type)
            {
                transition = (PetriNetTransition *) netItemList.at(i);
                transition->chosen = true;
                if(!SimulateStep(petriNet))
                {
                    if(error)
                    {
                        //nějaká sémantická chyba
                        return false;
                    }
                }
                else
                {
                    stepsDone++;
                }
                transition->chosen = false;
            }
            if(stepsDone >= maxSteps)
                break;
        }
    }

    if(error)
        return false;
    else
        return true;
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

/**
  * Vyhodnoti straz prechodu
  * @param map Seznam dvojic promennych a jejich hodnot vstupujicich do prechodu
  * @param guardGot Retezec straze
  */
bool Simulate::transitionGuard(QMap<PetriNetArrow *, int> *map, QString guardGot)
{
    QList<QString> guards = guardGot.split("&");
    QString guard;

        qDebug()<<map->keys();
        qDebug()<<map->values();
    for(int x = 0; x < guards.size(); x++)
    {
        /* Projdu vsechny casti vyroku rozdelene podle & */
        guard = guards.at(x);
        QRegExp rx("^\\s*([a-zA-Z]+)\\s*([<>=!]+)\\s*([a-zA-Z0-9]+)\\s*$");
        if(rx.indexIn(guard) != -1)
        {
            /* Odpovida predpisu */
            QString op1Var,op2Var,oper;
            oper = rx.cap(2);
            op1Var = rx.cap(1);
            op2Var = rx.cap(3);
            qDebug() << op1Var << oper << op2Var;
            int set1, set2, op1, op2;
            set1 = set2 = op1 = op2 = 0;
            QList<PetriNetArrow *> keys = map->keys();
            for (int i = 0; i < keys.size(); i++)
            {
                /* Projdu vsechny prvky */
                if (keys.at(i)->getVariable() == op1Var)
                {
                    /* Jsem na indexu, kde odpovida promena 1, takze vytahnu hodnotu */
                    op1 = map->value(keys.at(i));
                    set1 = 1;
                    qDebug() << "op1 prirazena hodnota" << op1;
                }

                bool b;
                op2Var.toInt(&b);
                if(b)
                {
                    /* Hodnota je ciselna, netreba hledat v promennych */
                    op2 = op2Var.toInt();
                    set2 = 1;

                }
                else if (keys.at(i)->getVariable() == op2Var)
                {
                    /* Jsem na indexu, kde odpovida promena 2, takze vytahnu hodnotu */
                    op2 = map->value(keys.at(i));
                    qDebug() << "op2 prirazena hodnota" << op2;
                    set2 = 1;
                }
            }
            if (set1 == 0 || set2 == 0 || !checkCondition(oper,op1,op2))
            {
                /* Jestli neplati byt jen jedno pravidlo, vracim chybu */
                qDebug() << "nevyhovuje pravidlo";
                return false;
            }
        }
        else
        {
            /* Nektere pravido neodpovida predpisu */
            return false;
        }
    }

    /* Jestli jsem se dostal az sem, je vsechno v poradku */
    qDebug() << "nasel jsem spravnou kombinaci";
    return true;
}


void Simulate::transitionAction(QMap<PetriNetArrow *, int> *input, QMap<PetriNetArrow*,int> *output, QString actionGot)
{
    QList<QString> actions = actionGot.split(",");
    QString action;

    foreach(action, actions)
    {
        /* Pro kazdou akci rozdelene podle , */
        QRegExp rx("^\\s*([a-zA-Z]+)\\s*=\\s*(.*)$");

        if(rx.indexIn(action) >= 0 )
        {
            /* Jestlize vyhovuje akce predpisu */
            QString target = rx.cap(1);
            qDebug()<< "target je"<<target;
            QString expression = rx.cap(2);
            QList<PetriNetArrow *> keysIn = input->keys();
            QList<PetriNetArrow *> keysOut = output->keys();

            /* Projdu akce prechodu a navazu vysledky */
            int result = 0;
            QList<QString> scitance = expression.split("+");
            int var = 0;
            for (int i = 0;i < scitance.size(); i++)
            {
                int partialResult = 0;
                /* Tady jsou jen pole, mezi kteryma je + */
                QList<QString> odcitance = scitance.at(i).split("-");
                if(odcitance.size() > 1)
                {
                    /* Kdyz jsou tam jeste nejake - */
                    for(int x = 0; x < odcitance.size(); x++)
                    {
                        bool test = false;
                        for(int n = 0; n < keysIn.size(); n++)
                        {
                            if (keysIn.at(n)->getVariable() == odcitance.at(x))
                            {
                                /* Jsem na indexu, kde odpovida promenne, takze vytahnu hodnotu */
                                var = input->value(keysIn.at(i));
                                test = true;
                            }
                        }
                        if(!test)
                        {
                            /* Promenna neni drive definovana -> chyba*/
                            error = true;
                            return;
                        }
                        partialResult -= var;
                    }
                }
                else
                {
                    bool test = false;
                    qDebug() << "hledam promennou"<<odcitance.at(0).trimmed();
                    for(int n = 0; n < keysIn.size(); n++)
                    {
                        if (keysIn.at(n)->getVariable() == odcitance.at(0).trimmed())
                        {
                            /* Jsem na indexu, kde odpovida promenne, takze vytahnu hodnotu */
                            var = input->value(keysIn.at(n));
                            test = true;
                        }
                    }

                    if(!test)
                    {
                        /* Promenna neni drive definovana -> chyba*/
                        qDebug() << "nedefinovana promenna";
                        error = true;
                        return;
                    }
                    partialResult += var;
                }

                result += partialResult;
            }

            /* Ulozim do spravne mista vysledny item */
            for(int i = 0; i < keysOut.size(); i++)
            {
                qDebug() << keysOut.at(i)->getVariable() << "a"<<target;
                if(keysOut.at(i)->getVariable() == target)
                {
                    qDebug() << "ukladam hodnotu" << result << "do promenne" << target;
                    ((PetriNetPlace *)(keysOut.at(i)->endItem()))->addToken(result);
                }
            }
        }
        else
        {
            qDebug() << "chyba parsovani pravidel";
            error = true;
            return;
            //chyba parsování action
        }
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

        factor *= tokenCount;
        i++;
        iter++;
    }
    return factor;
}

void Simulate::removeTokens(QMap<PetriNetArrow *, int> *map)
{
    QList<PetriNetArrow *> arrows = map->keys();

    foreach(PetriNetArrow * arrow, arrows)
    {
        PetriNetPlace * place = (PetriNetPlace *) arrow->startItem();
        place->removeToken((*map)[arrow]);
    }
}


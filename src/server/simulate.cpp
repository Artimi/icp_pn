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
bool Simulate::simulateStep(PetriNet *petriNet)
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
                //getPairs(inArrows,transition->getGuard(),&pairs);
               //if(!this->error)
                if(getPairs(inArrows,transition->getGuard(),&pairs))
                {
                    /* Vsecko probehlo v poradku, navazano je, pokracuju */
                    for(int arc = 0; arc < outArrows.count(); arc++)
                    {
                        output[outArrows[arc]]=0; //garbage hodnota
                    }

                    transitionAction(&pairs,&output,transition->getAction());
                    if(!this->error)
                    {
                        /* Odstranim tokeny, jestlize prechod probehl v poradku */
                        removeTokens(&pairs);
                    }
                    else
                    {
                        /* Jinak koncim */
                        return false;
                    }
                }
                else if(this->error)
                {
                    /* Zrejme semanticka chyba nekde v prubehu */
                    return false;
                }
                else
                {
                    /* Nepodarilo se navazat */
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
bool Simulate::getPairs(QList<PetriNetArrow *> inArrows,QString guard, QMap<PetriNetArrow*,int>* pairs)
{
    int arc;
    int i;
    int factor;

    //naplnění pairs key value
    for(arc = 0; arc < inArrows.count(); arc++)
    {
        PetriNetPlace * place = (PetriNetPlace *) inArrows[arc]->startItem();
        if(place->getTokens().count() > 0)
            (*pairs)[inArrows[arc]]=0;
    }

    int itemStates = getFactor(pairs,pairs->count());

    for (i = 0 ; i < itemStates; i++)
    {
        arc = 0;
        for(arc = 0; arc < inArrows.count(); arc++)
        {
            PetriNetPlace * place = (PetriNetPlace *) inArrows[arc]->startItem();
            if(place->getTokens().count() <= 0)
                continue;

            factor = getFactor(pairs,arc);

            if(factor == 0)
            {
                /* Pokud nemá žádný token, smažu ho ze vstupních, stejně tam nemá co přijít */
                pairs->remove(inArrows[arc]);
            }
            else
                (*pairs)[inArrows[arc]] = place->getTokens().at((i / getFactor(pairs,arc)) % place->getTokens().count());
        }

        if(transitionGuard(pairs,guard))
        {
            /* Nasel jsem, v pairs je správná kombinace tokenů */
            break;
        }

    }
    if (i == itemStates)
    {
        /* Pokud jsem prošel všechny stavy a žádný nevyhovoval je chyba, nemám co navázat */
        return false;
    }
    else
    {
        return true;
    }
}




/**
  * Odsimuluje celou petriho síť
  */
bool Simulate::simulateAll(PetriNet *petriNet)
{
    qDebug() << "Cela simulace";

    //int stepsDone = 0;
    //int maxSteps = 20;
    QList<PetriNetItem *> netItemList = petriNet->items();
    //PetriNetTransition * transition;

    /* Simulaci provadim podle prechodu, udelam si jejich seznam */
    QList<PetriNetTransition *> transitionList;
    for(int i = 0; i < netItemList.size(); i++)
    {
        if(netItemList.at(i)->type() == PetriNetTransition::Type)
        {
            transitionList.append((PetriNetTransition *) netItemList.at(i));
        }
    }

    /* Pocet prechodu */
    int transitionListCount = transitionList.size();
    /* Pocet prechodu, ktere byly proveditelne */
    int changed;

    while(!this->error)
    {
        qDebug()<<"vstoupil jsem do smycky";
        changed = 0;
        for (int i = 0; i < transitionListCount; i++)
        {
            /* Zkusim provest kazdy prechod */
            transitionList.at(i)->chosen = true;
            if (!simulateStep(petriNet))
            {
                qDebug()<<"neprobehl prechod";
                if(this->error)
                {
                    /* Semanticka chyba */
                    qDebug() << "skoncil se semantickou chybou";
                    return false;
                }
            }
            else
            {
                changed++;
            }
            transitionList.at(i)->chosen = false;
        }
        qDebug() << "changed na konci smycky" << changed;
        if (changed == 0)
        {
            /* Neprobehla zadna zmena v prechodech a ani jeden nebyl proveditelny -> uspesny konec */
            break;
        }
    }

    if(this->error)
    {
        return false;
    }
    else
    {
        return true;
    }
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
            QString target = rx.cap(1).trimmed();
            QString expression = rx.cap(2);
            QList<PetriNetArrow *> keysIn = input->keys();
            QList<PetriNetArrow *> keysOut = output->keys();

            /* Projdu akce prechodu a navazu vysledky */
            int result = 0;
            QList<QString> scitance = expression.split("+");
            int var = 0;
            for (int i = 0; i < scitance.size(); i++)
            {
                int partialResult = 0;
                /* Tady jsou jen pole, mezi kteryma je + */
                QList<QString> odcitance = scitance.at(i).split("-");
                if(odcitance.size() > 1)
                {
                    /* Kdyz jsou tam jeste nejake - */
                    for(int x = 0; x < odcitance.size(); x++)
                    {
                        QString symbol = odcitance.at(x).trimmed();
                        bool test = false;
                        for(int n = 0; n < keysIn.size(); n++)
                        {
                            /* Zjistim, co navazat do symbolu */
                            bool b;
                            symbol.toInt(&b);
                            if (b)
                            {
                                /* Konstanta, netreba hledat hodnotu */
                                qDebug() << "prirazena konstanta1" << symbol.toInt();
                                var = symbol.toInt();
                                test = true;
                                break;
                            }
                            else if (keysIn.at(n)->getVariable() == symbol)
                            {
                                /* Jsem na indexu, kde odpovida promenne, takze vytahnu hodnotu */
                                qDebug() << "prirazena promenne1"<<symbol<<"hodnota"<<input->value(keysIn.at(i));
                                var = input->value(keysIn.at(n));
                                test = true;
                                break;
                            }
                        }

                        if(!test)
                        {
                            /* Do symbolu neni co navazat */
                            qDebug() << "nedefinovana promenna1"<<symbol;
                            error = true;
                            return;
                        }

                        if (x == 0)
                        {
                            partialResult += var;
                        }
                        else {
                            partialResult -= var;
                        }
                    }
                }
                else
                {
                    QString symbol = odcitance.at(0).trimmed();
                    bool test = false;
                    qDebug() << "hledam promennou"<<symbol;
                    for(int n = 0; n < keysIn.size(); n++)
                    {
                        bool b;
                        symbol.toInt(&b);
                        if (b)
                        {
                            /* Konstanta, netreba hledat hodnotu */
                            qDebug() << "prirazena konstanta2" << symbol.toInt();
                            var = symbol.toInt();
                            test = true;
                            break;
                        }
                        else if (keysIn.at(n)->getVariable() == symbol)
                        {
                            /* Jsem na indexu, kde odpovida promenne, takze vytahnu hodnotu */
                            qDebug() << "prirazena promenne1"<<symbol<<"hodnota"<<input->value(keysIn.at(n));
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
//    qDebug() << "count"<<count << "factor" << factor;
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


/**
  * @file xmlhandler.cpp
  *
  * @brief Třída pro konverzi scény do xml
  * @author xsebek02, xsimon14
  */
#include "xmlhandler.h"

XMLHandler::XMLHandler(DiagramScene * scene,Message * message)
{
    myScene = scene;
    myMessage = message;
}

/**
  * Fce vytváří ze scény xml soubor obsahující petriho síť
  * @return QString obsahující xml petriho sítě
  */
QString XMLHandler::toStr()
{
    QString result;
    QXmlStreamWriter writer(&result);

    writePetriNet(&writer);

    return result;
}

/**
  * Ukládání xml do souboru
  * @param  file    soubor do, kterého se má ukládat
  */
void XMLHandler::saveNetToFile(QFile *file)
{
    QXmlStreamWriter writer(file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writePetriNet(&writer);
    writer.writeEndDocument();
}

/**
  * Načte xml ze souboru do myScene
  * @param  file    otevíraný soubor
  * @return 0 Ok, jinak chyba
  */
int XMLHandler::loadNetFromFile(QFile *file)
{
    QXmlStreamReader reader(file);
    reader.readNext();
    if(readPetriNet(&reader)!=0)
        return -1;
    return 0;
}

QString XMLHandler::writeMessage()
{
    QString result;
    QXmlStreamWriter writer(&result);

    writer.writeStartDocument();
    writer.writeStartElement("message");

    writer.writeTextElement("command",QString::number(myMessage->command));
    writer.writeStartElement("data");

    switch(myMessage->command)
    {
    case Message::SLOGIN:
        break;
    case Message::CLOGIN:
        writer.writeTextElement("user",myMessage->user);
        writer.writeTextElement("password",myMessage->password);
        break;
    case Message::WRONGLOGIN:
        break;
    case Message::LOGGED:
        break;
    case Message::CLIST:
        break;
    case Message::SLIST:
        writePetriNetList(&writer);
        break;
    case Message::SEND:
        writePetriNet(&writer);
        break;
    case Message::ERROR:
        writer.writeTextElement("error",myMessage->errorText);
        break;
    case Message::SAVE:
        writePetriNet(&writer);
        break;
    case Message::LOAD:
        writePetriNet(&writer); //ve scene bude uloženo jen name, author, version
        break;
    }

    writer.writeEndElement(); //data

    writer.writeEndElement(); //message
    writer.writeEndDocument();
    return result;
}

int XMLHandler::readMessage(QString str)
{
    QXmlStreamReader reader(str);

    reader.readNext(); // začátek dokumentu
    reader.readNext();

    if(reader.isStartElement() && reader.name()=="message")
    {
        reader.readNext();

        if(reader.isStartElement() && reader.name()=="command")
        {
            myMessage->command = (Message::Commands)reader.readElementText().toInt();
            qDebug()<<"command";
        }
        reader.readNext();

        if(reader.isStartElement() && reader.name()=="data")
        {
            reader.readNext();
            switch(myMessage->command)
            {
                case Message::CLOGIN:
                    while(!(reader.isEndElement() && reader.name() == "data"))
                    {
                        if(reader.isStartElement() && reader.name() == "user")
                            myMessage->user = reader.readElementText();
                        else if(reader.isStartElement() && reader.name() == "password")
                            myMessage->password = reader.readElementText();

                        reader.readNext();
                    }
                    break;
                case Message::SLIST:
                    readPetriNetList(&reader);
                    break;
                case Message::SEND:
                    readPetriNet(&reader);
                    break;
                case Message::ERROR:
                    if(reader.isStartDocument() && reader.name() == "error")
                        myMessage->errorText = reader.readElementText();
                    break;
                case Message::SAVE:
                    readPetriNet(&reader);
                    break;
                case Message::LOAD:
                    readPetriNet(&reader);
                    break;
            }
        }
    }

    if (reader.hasError())
    {
        return -1;
    }
    reader.clear();
    return 0;
}

/**
  * Zapisuje celou petriho síť ze scény do QXmlStreamWriteru
  * @param  writer  streamwriter, do kterého bude zapisováno
  */
void XMLHandler::writePetriNet(QXmlStreamWriter *writer)
{
    QList<QGraphicsItem *> list = myScene->items();

    writer->writeStartElement("petrinet");
    writer->writeAttribute("name",myScene->getName());
    writer->writeAttribute("version",myScene->getVersion());
    writer->writeAttribute("author",myScene->getAuthor());

    writer->writeTextElement("description",myScene->getDescription());

    foreach(QGraphicsItem * item, list)
    {
        if(item->type() == Place::Type)
        {
            Place * place = qgraphicsitem_cast<Place *>(item);
            writePlace(writer,place);
        }
        else if(item->type() == Transition::Type)
        {
            Transition * transition = qgraphicsitem_cast<Transition *>(item);
            writeTransition(writer,transition);
        }
    }
    //zapisuji se az posledni aby se posledni cetly -> vsechny objekty vytvořené
    foreach(QGraphicsItem * item, list)
    {
        if(item->type() == Arrow::Type)
        {
            Arrow * arrow = qgraphicsitem_cast<Arrow *>(item);
            writeArc(writer,arrow);
        }
    }
    writer->writeEndElement();// petrinet

}


/**
  * Zapíše informace o daném místě do streamwriteru
  * @param  writer  streamwriter, do kterého se bude zapisovat
  * @param  place   místo, které má být zapsáno
  */
void XMLHandler::writePlace(QXmlStreamWriter *writer, Place *place)
{
    writer->writeStartElement("place");
    writer->writeAttribute("name",place->getName());
    writer->writeTextElement("x",QString("%1").arg(place->x()));
    writer->writeTextElement("y",QString("%1").arg(place->y()));
    foreach(int token,place->getTokens())
    {
        writer->writeTextElement("token",QString::number(token));
    }
    writer->writeEndElement();//Place
}

/**
  * Zapíše informace o daném přechodu do streamwriteru
  * @param  writer  streamwriter, do kterého se bude zapisovat
  * @param  transition  popisovaný přechod
  */

void XMLHandler::writeTransition(QXmlStreamWriter *writer, Transition *transition)
{
    writer->writeStartElement("transition");
    writer->writeAttribute("name",transition->getName());
    writer->writeTextElement("x",QString("%1").arg(transition->x()));
    writer->writeTextElement("y",QString("%1").arg(transition->y()));
    writer->writeTextElement("guard", transition->getGuard());
    writer->writeTextElement("action",transition->getAction());
    writer->writeEndElement(); //transition
}

/**
  * Zapíše informace o dané hraně do streamwriteru
  * @param  writer  streamwriter, do kterého se bude zapisovat
  * @param  arrow   popisovaná hrana
  */
void XMLHandler::writeArc(QXmlStreamWriter *writer, Arrow *arrow)
{
    writer->writeStartElement("arc");
    writer->writeTextElement("startItem",arrow->startItem()->getName());
    writer->writeTextElement("endItem",arrow->endItem()->getName());
    writer->writeTextElement("variable",arrow->getVariable());
    writer->writeEndElement(); //arc
}

int XMLHandler::readPetriNetList(QXmlStreamReader *reader)
{

}

/**
  * Přečte celou petriho síť z reader a uloží ji do myScene
  * @param  reader  obsahuje stream reader s xml
  * @return 0 Ok, jinak chyba
  */
int XMLHandler::readPetriNet(QXmlStreamReader *reader)
{
    //reader->readNext(); //prectu zacatek dokumentu
    while(!reader->atEnd() && !reader->hasError())
    {
        if(reader->isStartElement() && reader->name()=="petrinet")
        {
            myScene->setName(reader->attributes().value("name").toString());
            myScene->setVersion(reader->attributes().value("version").toString());
            myScene->setAuthor(reader->attributes().value("author").toString());

            while(!(reader->isEndElement() && reader->name() == "petrinet") &&
                  !reader->hasError())
            {
                if (reader->isStartElement())
                {
                    if(reader->name() =="description")
                    {
                        myScene->setDescription(reader->readElementText());
                    }
                    else if (reader->name() == "place" )
                    {
                        readPlace(reader);
                    }
                    else if (reader->name() == "transition")
                    {
                        readTransition(reader);
                    }
                    else if (reader->name() == "arc")
                    {
                        readArc(reader);
                    }
                }
                reader->readNext();
            }

        }
        reader->readNext();
    }

    if (reader->hasError())
    {
        return -1;
    }
    reader->clear();
    return 0;
}

/**
  * Přečte místo v xml
  * @param  reader  obsahuje streamreader s xml
  * @return 0 ok, jinak chyba
  */
int XMLHandler::readPlace(QXmlStreamReader *reader)
{
    if(reader->name() == "place")
    {
        qreal x, y;
        x = y = 0;

        Place * place = new Place(myScene->myPlaceMenu);

        place->setName(reader->attributes().value("name").toString());
        reader->readNext();
        while (!(reader->isEndElement() && reader->name() == "place"))
        {
            if(reader->isStartElement())
            {
                if(reader->name() == "token")
                {
                    place->addToken(reader->readElementText().toInt());
                }
                else if(reader->name() == "x")
                {
                    x = reader->readElementText().toDouble();
                }
                else if (reader->name() == "y")
                {
                    y = reader->readElementText().toDouble();
                }
            }
            reader->readNext();
        }

        myScene->addItem(place);
        place->setPos(QPointF(x,y));
    }
    return 0;
}

/**
  * Přečte přechod z xml
  * @param  reader  obsahuje streamreader s xml
  * @return 0 ok, jinak chyba
  */
int XMLHandler::readTransition(QXmlStreamReader *reader)
{
    if(reader->name() == "transition")
    {
        qreal x, y;
        x = y = 0;

        Transition * transition = new Transition(myScene->myTransitionMenu);

        transition->setName(reader->attributes().value("name").toString());
        reader->readNext();
        while (!(reader->isEndElement() && reader->name() == "transition"))
        {
            if(reader->isStartElement())
            {
                if(reader->name() == "guard")
                {
                    transition->setGuard(reader->readElementText());
                }
                else if (reader->name() == "action")
                {
                    transition->setAction(reader->readElementText());
                }
                else if(reader->name() == "x")
                {
                    x = reader->readElementText().toDouble();
                }
                else if (reader->name() == "y")
                {
                    y = reader->readElementText().toDouble();
                }
            }
            reader->readNext();
        }

        myScene->addItem(transition);
        transition->setPos(QPointF(x,y));
    }
    return 0;
}

/**
  * Přečte hranu z Xml
  * @param  reader  streamreader s xml
  * @return 0 Ok, jinak chyba
  */
int XMLHandler::readArc(QXmlStreamReader *reader)
{
    if(reader->name() == "arc")
    {
        QString startItemStr;
        QString endItemStr;
        QString variable;

        reader->readNext();
        while(!(reader->isEndElement() && reader->name() == "arc"))
        {
            if(reader->isStartElement())
            {
                if(reader->name() == "startItem")
                {
                    startItemStr = reader->readElementText();
                }
                else if(reader->name() == "endItem")
                {
                    endItemStr = reader->readElementText();
                }
                else if(reader->name() == "variable")
                {
                    variable = reader->readElementText();
                }
            }
            reader->readNext();
        }
        DiagramItem * startItem =  myScene->getDiagramItem(startItemStr);
        DiagramItem * endItem = myScene->getDiagramItem(endItemStr);

        if(startItem == NULL || endItem == NULL)
        {
            return -1;
        }

        Arrow * arrow = new Arrow(startItem,endItem, myScene->myArrowMenu);
        arrow->setVariable(variable);

        startItem->addArrow(arrow);
        endItem->addArrow(arrow);
        myScene->addItem(arrow);
        arrow->updatePosition();
    }
    return 0;
}
/**
  * Vypíše všechny petriho sítě jednoho uživatele na serveru do xml
  */
void XMLHandler::writePetriNetList(QXmlStreamWriter *writer)
{

}



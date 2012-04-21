#include "xmlhandler.h"

XMLHandler::XMLHandler(DiagramScene * scene,QString name)
{
    myScene = scene;
}

QString XMLHandler::toXML()
{
    QString result;
    QXmlStreamWriter writer(&result);

    writePetriNet(&writer);
    return result;
}

void XMLHandler::saveToXMLFile(QFile *file)
{
        QXmlStreamWriter writer(file);
        writePetriNet(&writer);
}





void XMLHandler::writePetriNet(QXmlStreamWriter *writer)
{
    QList<QGraphicsItem *> list = myScene->items();

    writer->setAutoFormatting(true);

    writer->writeStartDocument();
    writer->writeStartElement("petrinet");
    writer->writeAttribute("name",myScene->name);
    writer->writeAttribute("version",myScene->version);
    writer->writeAttribute("author",myScene->author);

   writer->writeTextElement("description",myScene->description);

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
        else if(item->type() == Arrow::Type)
        {
            Arrow * arrow = qgraphicsitem_cast<Arrow *>(item);
            writeArc(writer,arrow);
        }
    }
    writer->writeEndElement();// petrinet
    writer->writeEndDocument();
}



void XMLHandler::writePlace(QXmlStreamWriter *writer, Place *place)
{
    writer->writeStartElement("place");
    writer->writeAttribute("name",place->getName());

    writer->writeTextElement("x",QString("%1").arg(place->x()));

    writer->writeTextElement("y",QString("%1").arg(place->y()));

    foreach(int token,place->getTokens())
    {
        writer->writeTextElement("token",QString(token));
    }
    writer->writeEndElement();//Place
}

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

void XMLHandler::writeArc(QXmlStreamWriter *writer, Arrow *arrow)
{
    writer->writeStartElement("arc");

    writer->writeTextElement("startItem",arrow->startItem()->getName());

    writer->writeTextElement("endItem",arrow->endItem()->getName());

    writer->writeTextElement("variable",arrow->getVariable());

    writer->writeEndElement(); //arc
}



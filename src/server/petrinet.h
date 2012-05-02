#ifndef PETRINET_H
#define PETRINET_H

#include <QtCore>
#include "petrinetitem.h"

class PetriNet
{
public:
    PetriNet();
    ~PetriNet();

    QString getName()
    {return name;}
    QString getVersion()
    {return version;}
    QString getAuthor()
    {return author;}
    QString getDescription()
    {return description;}

    QList<PetriNetItem *> items()
    {return myItems;}

    void addItem(PetriNetItem *item);

    void setName(QString str)
    {name = str;}
    void setVersion(QString str)
    {version = str;}
    void setAuthor(QString str)
    {author = str;}
    void setDescription(QString str)
    {description = str;}

private:
    QString name;
    QString version;
    QString author;
    QString description;

    QList<PetriNetItem *> myItems;
};

#endif // PETRINET_H

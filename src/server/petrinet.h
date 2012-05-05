/**
  * @file petrinet.h
  *
  * @brief Implementace petriho sítě
  * @author xsebek02 xsimon14
  */
#ifndef PETRINET_H
#define PETRINET_H

#include <QtCore>
#include "petrinetitem.h"
#include "petrinetobject.h"
#include "petrinetplace.h"
#include "petrinettransition.h"

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


    PetriNetItem* getPetriNetItem(QString name);

private:
    QString name;
    QString version;
    QString author;
    QString description;

    QList<PetriNetItem *> myItems;
};

#endif // PETRINET_H

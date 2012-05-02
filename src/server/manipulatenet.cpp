/**
  * @file manipulatenet.cpp
  *
  * @brief Třída pro manipulaci se sitemi na strane serveru
  * @author xsebek02, xsimon14
  */
#include "manipulatenet.h"

/**
  * Inicializuje manipulacni strukturu
  */
ManipulateNet::ManipulateNet()
{
    error = false;
}


/**
  * Ulozi sit do souboru s posledni verzi
  * @param name Jmeno site
  * @param username Uzivatelske jmeno
  * @param
  */
void ManipulateNet::saveNet(QString name, QString username,XMLHandler *xmlhandler)
{
    int version = investigateVersion(name, username);

    if (version == -1)
    {
        /* Nastala nejaka chyba pri zjistovani verze */
        qDebug() << "Error while investigating number of version";
        error = true;
        return;
    }

    QDir nets("nets");
    QDir userdir("nets/" + username);
    if (!userdir.exists())
    {
        /* Vytvori uzivatelsky adresar, pokud neexistuje */
        qDebug() << "Created dir \"nets/" << userdir << "\"";
        nets.mkdir(username);
    }

    QFile file(username + "_" + QString::number(version));
    if (!file.open(QIODevice::WriteOnly))
    {
        /* Nepodarilo se otevrit soubor */
        qDebug() << "Error while opening file for writing";
        error = true;
        return;
    }
    xmlhandler->saveNetToFile(&file);
    file.close();
}

/**
  * Vrati petriho sit zvoleneho jmena
  * @param name Jmeno site
  * @param username Uzivatelske jmeno
  * @param version Verze site, ktera ma byt nactena
  * @param resultNet Adresa site, kam se ulozi vysledek
  */
void ManipulateNet::loadNet(QString name, QString username, QString version, PetriNet * resultNet)
{
    QString filename = "nets/" + username + "/" + name + "_" + version; //.xml?
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        /* Soubor se nepodarilo otevrit pro cteni */
        qDebug() << "Error while opening file for reading";
        error = true;
        return;
    }
    else
    {
        /* Soubor se podarilo otevrit */
        XMLHandler xmlhandler;
        xmlhandler.setPetriNet(resultNet);
        if(xmlhandler.loadNetFromFile(&file) != 0)
        {
            /* Nepodarilo se rozparsovat soubor */
            qDebug() << "Error while parsing the file into the net";
            error = true;
        }
        return;
    }
}

/**
  * Vrati seznam vsech siti ulozenych danym uzivatelem
  * @param username Uzivatelske jmeno
  */
 QList<PetriNet *> ManipulateNet::getNetList(QString username)
 {
     QList<PetriNet *> result;
     QString userdir = "nets/" + username;
     QDir dir(userdir);
     if (!dir.exists())
     {
         /* Uzivatel nema jeste nic ulozeneho */
         return result;
     }
     else
     {
         /* Uzivatel ma slozku, pridam vsechny site do seznamu */
         QStringList files = dir.entryList();
         for (int i = 0; i< files.size(); i++)
         {
             //nactu z xml do PetriNet
             PetriNet * net;
             QFile file(userdir + "/" + files.at(i));
             if (!file.open(QIODevice::ReadOnly))
             {
                 /* Nepodarilo se otevrit soubor pro cteni */
                 qDebug() << "Error while opening file for reading";
                 error = true;
                 return result;
             }
             XMLHandler xmlhandler;
             xmlhandler.setPetriNet(net);
             if(xmlhandler.loadNetFromFile(&file) != 0)
             {
                 qDebug() << "Error while parsing the file into the net";
                 error = true;
                 file.close();
                 return result;
             }

             result.append(net);
             file.close();
         }
         return result;
     }
 }

/**
  * Zjisti cislo verze, kterou dostane sit zvoleneho jmena
  * @param name Jmeno site
  * @param username Uzivatelske jmeno
  */
int ManipulateNet::investigateVersion(QString name, QString username)
{
    QDir nets("nets");
    QDir userdir("nets/" + username);
    if (!userdir.exists())
    {
        /* Uzivatel jeste nema nic ulozeno */
        nets.mkdir(username);
        return 0;
    }
    else
    {
        /* Uzivatel ma uz neco ulozeni */
        QStringList files = userdir.entryList();
        int lastVersion = 0;
        QRegExp rx;
        rx.setPattern("^(.+)_([^_]+)$");
        QString netname, version;
        int ver;
        bool b;
        for(int i = 0; i < files.size();i++)
        {
            /* Projedu vsechny soubory */
            if (rx.indexIn(files.at(i)) != -1)
            {
                /* Vsechny soubory by mely odpovidat predpisu */
                netname = rx.cap(1);
                if (netname == name)
                {
                    /* Soubor je spravna sit */
                    version = rx.cap(2);
                    qDebug() << version;
                    ver = version.toInt(&b,10);
                    if(b && ver > lastVersion)
                    {
                        lastVersion = ver;
                    }
                }
            }
            else
            {
                /* Soubor neodpovida predpisu, je chybny?*/
                return -1;
            }
        }
        return lastVersion;
    }
}

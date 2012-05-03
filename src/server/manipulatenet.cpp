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
    apppath = QFileInfo(QCoreApplication::applicationFilePath()).path() + "/";
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

    QDir nets(apppath + "nets");
    QString userdirname = apppath + "nets/" + username;
    QDir userdir(userdirname);
    if (!userdir.exists())
    {
        /* Vytvori uzivatelsky adresar, pokud neexistuje */
        if(!nets.mkpath(username))
        {
            /* Nepodarilo se vytvorit adresar uzivatele */
            qDebug() << "Error while creating user dir";
            error = true;
            return;
        }
        qDebug() << "Created dir" << userdirname;
    }

    QString filename = userdirname + "/" + name + "_" + QString::number(version) + ".xml";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        /* Nepodarilo se otevrit soubor */
        qDebug() << "Error while opening file for writing";
        error = true;
        return;
    }

    qDebug() << "The net has been saved into file " << filename;

    xmlhandler->getPetriNet()->setAuthor(username);
    xmlhandler->getPetriNet()->setVersion(QString::number(version));
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
    QString filename = apppath + "nets/" + username + "/" + name + "_" + version + ".xml"; //.xml?
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
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
 void ManipulateNet::getNetList(QString username, QList<PetriNet*>* result)
 {
     //QList<PetriNet *> result;
     result->clear();
     QString userdir = apppath + "nets/" + username;
     QDir dir(userdir);
     if (!dir.exists())
     {
         /* Uzivatel nema jeste nic ulozeneho */
         return;
     }
     else
     {
         /* Uzivatel ma slozku, pridam vsechny site do seznamu */
         QStringList files = dir.entryList();
         QRegExp rx;
         rx.setPattern("^(.+)_([^_]+).xml$");
         for (int i = 0; i< files.size(); i++)
         {
             if (rx.indexIn(files.at(i)) != -1)
             {
                 /* Zajimaji me jen site, ktere odpovidaji predpisu */
                 PetriNet *net = new PetriNet;
                 QFile file(userdir + "/" + files.at(i));
                 if (!file.open(QIODevice::ReadOnly))
                 {
                     /* Nepodarilo se otevrit soubor pro cteni */
                     qDebug() << "Error while opening file for reading";
                     error = true;
                     return;
                 }
                 qDebug() << userdir + "/" + files.at(i);
                 XMLHandler xmlhandler;
                 xmlhandler.setPetriNet(net);
                 if(xmlhandler.loadNetFromFile(&file) != 0)
                 {
                     qDebug() << "Error while parsing the file into the net";
                     error = true;
                     file.close();
                     return;
                 }

                 *result << net;
                 file.close();
             }
         }
         return;
     }
 }

/**
  * Zjisti cislo verze, kterou dostane sit zvoleneho jmena
  * @param name Jmeno site
  * @param username Uzivatelske jmeno
  */
int ManipulateNet::investigateVersion(QString name, QString username)
{
    QDir nets(apppath + "nets");
    QDir userdir(apppath + "nets/" + username);
    if (!userdir.exists())
    {
        /* Uzivatel jeste nema nic ulozeno */
        nets.mkdir(username);
        return 1;
    }
    else
    {
        /* Uzivatel ma uz neco ulozeni */
        QStringList files = userdir.entryList();
        int lastVersion = 0;
        QRegExp rx;
        rx.setPattern("^(.+)_([^_]+).xml$");
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
                    ver = version.toInt(&b,10);
                    if(b && ver > lastVersion)
                    {
                        lastVersion = ver;
                    }
                }
            }

        }
        return lastVersion + 1;
    }
}

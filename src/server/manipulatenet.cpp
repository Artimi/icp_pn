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
  */
void ManipulateNet::saveNet(QString name, QString username,XMLHandler *xmlhandler)
{
    int version = investigateVersion(name, username);

    if (version == -1)
    {
        /* Nastala nejaka chyba pri zjistovani verze */
        qDebug() << "chyba pri zjistovani cisla verze";
        error = true;
        return;
    }

    qDebug()  << "verze "<< version;

    QDir nets("nets");
    QDir userdir("nets/" + username);
    if (!userdir.exists())
    {
        qDebug() << "vytvoren adresar nets/" << userdir;
        nets.mkdir(username);
    }

    QFile file(username + "_" + QString::number(version));
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "chyba pri otevreni souboru pro zapis";
        return;
    }
    xmlhandler->saveNetToFile(&file);
    file.close();
}



/**
  * Vrati petriho sit zvoleneho jmena
  * @param  name    Jmeno site
  */
void ManipulateNet::loadNet(QString name, QString username, XMLHandler *xmlhandler)
{

}

/**
  * Zjisti cislo verze, kterou dostane sit zvoleneho jmena
  * @param  name    Jmeno site
  * @param  username    Uzivatelske jmeno
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
            if (rx.indexIn(files.at(i)) != -1)
            {
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

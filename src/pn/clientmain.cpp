/**
  * @file main.cpp
  * @author xsebek02 xsimon14
  * @brief Tento soubor spouští aplikaci Qt a předává jí řízení
  */


/**

@mainpage Dokumentace k editoru a simulátoru vysokoúrovňových Petriho sítí

Datum: 5.5.2012
Autoři: Petr Šebek & Martin Šimon


\section popis_sec Popis aplikace:

Aplikace je založena na konceptu klient/server, kdy klient slouží jako editor
vysokoúrovňových Petriho sítí s funkcemu jako je uložení a opětovné načtení těchto sítí,
úprava stávající sítě a tvorba úplně nové a úpravy metainformací o síti jako jsou jméno
autora, název sítě či její popis. Strana klienta pak poskytuje možnost uložit sítě
na vzdáleném úložišti, jejich opětovné načtení, ale především simulaci sítí ve dvou
základních rovinách - po jednom kroku nebo až do konce.


\section prerekvizity_sec Prerekvizity:

Qt verze alespoň 4.7


\section preklad_sec Překlad:

Po rozbalení archivu se samotná aplikace přeloží příkazem <br />
$ make


\section spusteni_sec Spuštění:

Pokud překlad proběhl bez chyb, v složkách src/pn/ a src/server/ se vytvořily binární
spustitelné soubory pn2012 a serve2012. Jejich spuštění se provede buď příkazem <br />
$ make runserver <br />
pro server, nebo <br />
$ make run <br />
pro klient. <br />

Alternativní způsob spuštění klienta je spustit ve složce src/pn/ příkaz <br />
$ ./pn2012 <br />
případně pro server ve složce src/server/ spustit příkaz <br />
$ ./server2012 <br />

\section prihlaseni_sec Přihlášení na server:
Na server se lze přihlásit pod jménem <b>user</b> s heslem <b>pass</b> nebo být
v roli hosta jako uživatel guest

*/

#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    
    return a.exec();
}

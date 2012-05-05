Editor a simulátor vysokoúrovňových Petriho sítí
---Projekt do předmětu ICP----

Datum: 5.5.2012
Autoři: Petr Šebek & Martin Šimon

=========================================================================================


Popis aplikace:
-------------------------
Aplikace je založena na konceptu klient/server, kdy klient slouží jako editor
vysokoúrovňových Petriho sítí s funkcemu jako je uložení a opětovné načtení těchto sítí,
úprava stávající sítě a tvorba úplně nové a úpravy metainformací o síti jako jsou jméno
autora, název sítě či její popis. Strana klienta pak poskytuje možnost uložit sítě
na vzdáleném úložišti, jejich opětovné načtení, ale především simulaci sítí ve dvou
základních rovinách - po jednom kroku nebo až do konce.


Prerekvizity:
-------------------------
Qt verze alespoň 4.7


Překlad:
-------------------------
Po rozbalení archivu se samotná aplikace přeloží příkazem
$ make


Spuštění:
-------------------------
Pokud překlad proběhl bez chyb, v složkách src/pn/ a src/server/ se vytvořily binární
spustitelné soubory pn2012 a serve2012. Jejich spuštění se provede buď příkazem
$ make runserver
pro server, nebo
$ make run
pro klient.

Alternativní způsob spuštění klienta je spustit ve složce src/pn/ příkaz
$ ./pn2012
případně pro server ve složce src/server/ spustit příkaz
$ ./server2012


Poznámky:
-------------------------





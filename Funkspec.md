# Funkcionális specifikáció

## 1. Vágyálomrendszer leírása

Egy olyan félanalóg órát tervezünk készíteni, amelyben charlieplexing megoldást alkalmazunk. Az órán potenciométerek és gombok segítségével tudjuk beállítani a pontos időt vagy egy ébresztőt - mikrokontroller pedig egy Arduino Nano lesz.

## 2. Alkatrész lista:

### Minden alkatrész be lett szerezve

* 1db Arduino Micro
* 13db egyszínű LED
* 2db 7-szegmenses kijelző
* 2db gomb
* 2db potméter
* 1db hangszóró
* ellenállások ledekhez
* jumper kábelek

## 3. Használati esetek (működés)

* Idő állító gomb lenyomásánál
  * óra állítás potméterrel
  * perc állítás potméterrel
* Ébresztő állító gomb lenyomásánál
  * óra állítás potméterrel
  * perc állítás potméterrel
  * az ébresztő állító gomb felengedésénél visszaáll a pontos idő
  * amikor a jelenlegi idő megegyezik a beállított idővel, megszólal az ébresztő (hangszóró)
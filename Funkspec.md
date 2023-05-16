
# Funkcionális specifikáció

## 1. Vágyálomrendszer leírása

Egy olyan félanalóg órát tervezünk készíteni, amelyben charlieplexing megoldást alkalmazunk. Az órán potenciométerek és gombok segítségével tudjuk beállítani a pontos időt vagy egy ébresztőt - mikrokontroller pedig egy Arduino Nano lesz.

## 2. Alkatrész lista:

### Minden alkatrész be lett szerezve

* 1db Arduino UNO
* 12db egyszínű LED
* 2db 7-szegmenses kijelző
* 2db gomb
* 2db potméter
* 1db hangszóró
* ellenállások
* jumper kábelek
* 2db 27HC595 8bit register

## 3. Használati esetek (működés)

* Mód állító gomb lenyomásánál
  * 3 mód között vált: 
	  * Óra: potméreteket nem lehet használni
	  * Idő állítás: potméterekkel időt állítunk
	  * Ébresztő állítás: potméterekkel ébresztőt állítunk, óra módra visszalépésnél visszaáll a rendes idő
* Ébresztő gomb lenyomásánál
  * ha szól az ébresztő, kikapcsolja
  * 5mp-ig nyomvatartásnál törli a beállított ébresztőt

## 4. Képernyőtervek (sematikus rajz és CAD tervrajz)

### 4.1: Sematikus rajz:

![SematikusRajz](https://github.com/mviktor02/robotika/blob/main/img/SematikusRajz.JPG)

### 4.2: CAD tervrajz:

![CADRajz](https://github.com/mviktor02/robotika/blob/main/img/CADTervrajz.png)

## 5. Forgatókönyvek (állapotátmenet gráf)

### 5.1: jelölések

#### 5.1.1 Állapotok jelölése:
 
 * A: jár az óra ébresztő nélkül
 * É: ébresztő
 * B: Ébresztő bekapcsolása
 * Sz: Ébresztő szól
 * I: Idő beállítása

#### 5.1.2 Gomblenyomások jelölése:
 
 * m: módváltó gomb
 * é: ébresztő állító gomb
 
 ### 5.2 Állapotátmenet gráf:
![AllapotatmenetGraf](https://github.com/mviktor02/robotika/blob/main/img/allapotatmenetGraf.jpg)

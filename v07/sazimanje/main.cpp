// Napraviti konkurentni program koji simulira dinamičku dodelu memorijskog prostora procesima.
// Memorija je podeljena na memorijske lokacije. Procesu je potreban određeni broj slobodnih UZASTOPNIH memorijskih lokacija da bi se smestio u memoriju. Proces zauzima prvi slobodan blok memorije koji pronađe. Kada proces završi sa radom, oslobađaju se lokacije koje je koristio.
//
// U opisanom scenariju, kako procesi završavaju sa radom, dolazi do pojave "rupa" u memoriji (slobodni blokovi memorije). Može se desiti da ukupan broj slobodnih memorijskih lokacija bude dovoljan da se novi proces smesti, ali da se te slobodne lokacije ne nalaze u sukcesivnim lokacijama memorije. Zato je potrebno izvršiti SAŽIMANJE memorije. Pri sažimanju memorije, trenutni procesi u memoriji se pomeraju tako da zauzimaju uzastopne lokacije pocevši od prve. Na taj način, na kraju memorije ostaju sve slobodne lokacije kao jedan blok uzastopnih lokacija.
//
// Memorija je predstavljena datom klasom Radna_memorija. Pri instanciranju se prosleđuje broj lokacija u memoriji. Procesi su predstavljeni nitima. Telo niti je funkcija proces. Svaki proces dobija kao parametar broj lokacija koje su mu potrebne da se smesti u memoriju. Procesi zauzimaju memoriju pozivom operacije "koristi" klase Radna_memorija. Ukoliko ne postoji dovoljno uzastopnih slobodnih memorijskih lokacija koliko procesu treba, proces mora da čeka. Kada ima dovoljno lokacija, proces zauzima lokacije. Nakon toga se izvršava. Izvršavanje simulirati uspavljivanjem niti na broj sekundi definisan parametrom "trajanje". Nakon toga, proces oslobađa lokacije koje je koristio i traži sažimanje memorije.
//
// Sažimanje memorije vrši posebna nit predstavljena funkcijom os. Samo sažimanje vrši se u metodi "sazimanje" klase radna memorija. U ovoj metodi, mehanizam zadužen za sažimanje čeka dok neki proces ne zatraži sažimanje. Kada se sažimanje zatraži, vrši se sažimanje memorije na ranije opisani način. Nakon sažimanja, moguće je da u memoriji postoji veći slobodan blok nego ranije, pa se neki od procesa koji su čekali na memoriju sada može smestiti.
//
// U okviru sažimanja NIJE dozvoljeno vršiti sortiranje memorijskih lokacija kao skraćeni postupak za izbacivanje slobodnih lokacija na kraj. Razlog je u tome što će sortiranje potencijalno ispremeštati mnogo više lokacija nego što bi to uradio postupak sažimanja koji je prethodno opisan (a u realnom scenariju, svako pomeranje lokacija je operacija sa značajnim trajanjem).
//
// Npr. ukoliko je izgled memorije pre sažimanja:
//
//	-1 -1 3 3 3 1 1 -1 -1 2 2
//
//	Nakon sažimanja izgled memorije treba da bude:
//
//	3 3 3 1 1 2 2 -1 -1 -1 -1
//
//	a NE:
//
//	1 1 2 2 3 3 3 -1 -1 -1 -1 ili 3 3 3 2 2 1 1 -1 -1 -1
//

#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "radna_memorija.h"

using namespace std;
using namespace chrono;

mutex term_m;

// Funkcija niti koja simulira izvršenje procesa:
void proces(Radna_memorija& rm, int id_procesa, int br_lokacija_procesa, int trajanje, int kasnjenje) {
    this_thread::sleep_for(seconds(kasnjenje));
    rm.getDijagnostika().proces_krenuo(id_procesa, br_lokacija_procesa);
	rm.koristi(id_procesa, br_lokacija_procesa, trajanje);
}

// Funkcija niti koja simulira servis operativnog sistema koji se bavi sažimanjem memorijskih lokacija:
void os(Radna_memorija& rm) {
	while (true) {
        rm.sazimanje();
	}
}

const int BROJ_PROCESA = 5;
const int UKUPNO_OKVIRA = 54;

// Scenario je napravljen tako da proces 5 ne bi mogao da se ubaci u memoriju da ne postoji sažimanje memorijskih lokacija:
void testirajSve() {
    Dijagnostika d;
	Radna_memorija rm(d, UKUPNO_OKVIRA);

	int velicine[] =  {20, 14, 18,  8, 16};
	int trajanje[] =  { 5,  2,  5,  4,  4};
	int kasnjenje[] = { 0,  1,  2,  4,  5};

    thread os_thread(os, ref(rm));
	thread procesi[BROJ_PROCESA];

	for (int i = 0; i < BROJ_PROCESA; i++)
		procesi[i] = thread(proces, ref(rm), i+1, velicine[i], trajanje[i], kasnjenje[i]);

	for (int i = 0; i < BROJ_PROCESA; i++)
		procesi[i].join();

    os_thread.detach();
}

int main() {
    testirajSve();

    exit(0);   //exit 0 - predstavlja signal detach-ovanoj niti da prekine rad jer više nije neophodna
}

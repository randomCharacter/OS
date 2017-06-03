// Napisati konkurentni program koji simulira raspoređivanje po algoritmu povratne sprege u jednoprocesorskom sistemu.
//
// Procesi su predstavljeni nitima. Da bi se izvršili pozivaju metodu "izvrsi" deljene promenljive tipa Rasporedjivac. Svaki proces sadrži određeni broj naredbi. Naredbe se izvršavaju jedna po jedna. Za svaku naredbu, proces mora da sačeka da procesor bude slobodan. Izvršavanje jedne naredbe traje 300 ms. Nakon izvršavanja naredbe, proces odlazi u stanje spreman, a drugi spreman proces dobija procesor.
//
// Procesi su klasifikovani prema prioritetu. 0 je najviši prioritet dok je N najmanji!
//
// Za svaki prioritet, postoji odvojen red čekanja na procesor. Prednost u korišćenju procesora imaju procesi sa višim prioritetom. Među procesima istog prioriteta, sledeći proces se bira po FIFO principu.
//
// Prioritet procesa se određuje dinamički u toku izvršavanja procesa. Na početku rada, proces ima najviši prioritet. Kada izvrši naredbu, prelazi na jedan nivo prioriteta ispod. Nakon svake naredbe, procesu se smanjuje prioritet za jedan nivo, dok ne dođe do najnižeg prioriteta. Ako je došao do najnižeg prioriteta, na tom prioritetu ostaje do kraja izvršavanja.
//

#include <thread>

#include "rasporedjivac.h"

using namespace std;

// Funkcija koju koriste niti koje simuliraju izvršenje procesa:
void proces(Rasporedjivac& r, int id_procesa, int broj_naredbi) {
    r.getDijagnostika().proces_kreiran(id_procesa, broj_naredbi);

	r.izvrsi(id_procesa, broj_naredbi);

	r.getDijagnostika().proces_zavrsio(id_procesa);
}

const int BROJ_PROCESA = 5;
const int BROJ_PRIORITETA = 5;

void testirajSve() {
	srand(time(NULL));

    Dijagnostika d;
	Rasporedjivac r(d, BROJ_PRIORITETA);
	thread procesi[BROJ_PROCESA];

	for (int i = 0; i < BROJ_PROCESA; i++)
		procesi[i] = thread(proces, ref(r), i+1, rand()%5 + 1);

	for (int i = 0; i < BROJ_PROCESA; i++)
		procesi[i].join();
}

int main() {
    testirajSve();

    return 0;
}

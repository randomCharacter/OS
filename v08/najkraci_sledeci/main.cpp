// Napisati konkurentni program koji simulira raspoređivanje po sistemu SPN (Najkraći proces sledeći).
//
// Ovo je politika raspoređivanja bez prekidanja procesa koji je zauzeo procesor. Drugim rečima, proces koji je zauzeo procesor izvršava se u potpunosti pre nego što procesor može da se preključi na neki od narednih procesa. Kod preključivanja uvek treba voditi računa da procesor zauzme onaj proces koji ima najkraće očekivano vreme izvršavanja (mereno u stotinama milisekundi).
//
// Klasa Racunar predstavlja deljenu promenljivu kojoj pristupaju niti procesi i rasporedjivac.
//
// Procesi su predstavljeni nitima čije je telo data funkcija proces. Proces se izvršava pozivom metode izvrsi klase Racunar. Izvrsavanje će trajati onoliko stotina milisekundi kolika je vrednost parametra trajanje. Ukoliko je procesor zauzet, proces ulazi u čekanje kako bi ga raspoređivač naknadno rasporedio (kada dođe njegov red na izvršavanje).
//
// Kada se jedan proces završi, potrebno je da se rasporedi naredni proces. Raspoređivanje vrši posebna nit predstavljena funkcijom rasporedjivac. Ova nit se aktivira kada je potrebno izvršiti raspoređivanje narednog procesa a raspoređivanje se vrši prema SPN algoritmu opisanom na početku zadatka.
//

#include <thread>

#include "racunar.h"

using namespace std;
using namespace chrono;

mutex term_m;

// Funkcija koju izvršavaju niti koje simuliraju izvršenje procesa:
void proces(Racunar& r, int id_procesa, int trajanje) {
    r.getDijagnostika().proces_kreiran(id_procesa, trajanje);

    r.izvrsi(id_procesa, trajanje);
}

// Funkcija niti koju izvršava nit koja simulira raspoređivač operativnog sistema:
void rasporedjivac(Racunar& r) {
    r.rasporedjuj();
}

void testirajSve() {
    Dijagnostika d;
    Racunar r(d);
    thread tr(rasporedjivac, ref(r));
    tr.detach();
    int trajanja[] = {3, 2, 5, 7, 4, 9, 6};
    thread procesi[7];
    for (int i = 0; i < 7; i++) {
        procesi[i] = thread(proces, ref(r), i+1, trajanja[i]);
    }
    for (int i = 0; i < 7; i++)
        procesi[i].join();
}

int main() {
    testirajSve();

    exit(0); //exit 0 - predstavlja signal detach-ovanoj niti da prekine rad jer više nije neophodna
}

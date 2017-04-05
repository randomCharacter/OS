// Modelovati koriscenje zaletista na atletskom mitingu.
// Isto zaletiste koriste dve discipline: skok u dalj i bacanje koplja.
// Zaletiste ne mogu istovremeno da koriste dva takmicara.
// Discipline se naizmenicno smenjuju na zaletistu (jedan skakac u dalj, pa jedan bacac koplja i tako redom).
//
// Skok u dalj za jednog takmicara traje 1 sekundu. Bacanje koplja 2 sekunde.
// Metodu skaci poziva skakac u dalj. Metoda vraca duzinu u metrima koliko je takmicar skocio
// (izmedju 0 i 9 metara moze skociti) i koliko je ukupno trebalo vremena da zavrsi skok
// (koliko je zajedno trajalo cekanje i skakanje).
// Metodu baciKoplje poziva bacac koplja. Metoda vraca duzinu u metrima koliko je takmicar bacio koplje
// (izmedju 0 i 100 metara moze baciti) i koliko je ukupno trebalo vremena da zavrsi bacanje koplja
// (koliko je zajedno trajalo cekanje i bacanje koplja).
//
// Implementirati oznacene metode klase Atletska_staza u fajlu staza.h

#include <thread>

#include "staza.h"

using namespace std;

void skakac(Atletska_staza& staza, int rbr) {
	Povratna_vrednost rez = staza.skaci(rbr);
}

void bacac(Atletska_staza& staza, int rbr) {
	Povratna_vrednost rez = staza.baciKoplje(rbr);
}

const int BROJ_SKAKACA = 10;
const int BROJ_BACACA = 10;

void testirajSve() {
    Takmicar takmicar;
    Atletska_staza staza(takmicar);
    thread skakaci[BROJ_SKAKACA];                            //Jednak broj skakaca i bacaca, radi lakseg pracenja programa.
    thread bacaci[BROJ_BACACA];

    for (int i = 0; i < 10; ++i) {
        skakaci[i] = thread(skakac, ref(staza), i + 1);
        bacaci[i] = thread(bacac, ref(staza), i + 1);
    }

    for (int i = 0; i < BROJ_BACACA; ++i)
        bacaci[i].join();

    for (int i = 0; i < BROJ_SKAKACA; ++i)
        skakaci[i].join();

}

int main() {
    testirajSve();

    return 0;
}

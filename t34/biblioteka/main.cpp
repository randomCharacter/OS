// Modelovati iznajmljivanje jedne knjige u biblioteci. Biblioteka poseduje N primeraka ove knjige.
// Ovaj parametar se prosledjuje pri inicijaliziciji biblioteke.
// Clan iznajmljuje primerak pozivom metode iznajmi().
// Ukoliko su svi primerci trenutno na citanju, clan biblioteke mora da saceka da neki primerak bude vracen u biblioteku.
// Primerak moze da bude kod clana na citanju neki slucajan vremenski period, koji nije duzi od 4 sekunde.
// Nakon citanja, clan vraca primerak u biblioteku pozivom metode vrati().
//
// Implementirati oznacene metode klase Biblioteka u fajlu biblioteka.h

#include <iostream>
#include <thread>

#include "biblioteka.h"

using namespace std;
using namespace chrono;

void clan(Biblioteka& b, int br_clanske_karte) {
    b.iznajmi(br_clanske_karte);
    this_thread::sleep_for(seconds(rand()%4 + 1));
    b.vrati(br_clanske_karte);
}

void testirajSve() {
    int broj_primeraka = 3;
    int broj_clanova = 10;

    Primerak p;
    Biblioteka b(p, broj_primeraka);
    thread clanovi[broj_clanova];

    for (int i = 0; i < broj_clanova; ++i) {
        clanovi[i] = thread(clan, ref(b), i+1);
    }

    for (int i = 0; i < broj_clanova; ++i) {
        clanovi[i].join();
    }
}

int main() {
    testirajSve();

    return 0;
}

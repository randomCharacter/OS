#ifndef PARKING_H_INCLUDED
#define PARKING_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "automobil.h"

using namespace std;

class Parking {
private:
    Automobil& automobil;
    mutex m;
    condition_variable cv;
    int mesta;

public:
    Parking(Automobil& a, int kapacitet) : automobil(a) {
        mesta = kapacitet;
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kako bi automobil pokusao da se parkira.
    // Ako je parking zauzet, izvrsenje ove metode ce trajati dok se parking ne oslobodi i auto se parkira.
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.ceka kada je parking zauzet i auto ne moze da se parkira.
    // Potrebno je pozvati metodu automobil.parkira kada auto uspe da se parkira.
    void udji(int rbr) {
        unique_lock<mutex> l(m);
        // Ako nema mesta
        while (mesta == 0) {
            automobil.ceka(rbr);
            cv.wait(l);
        }
        // Kad se mesto oslobodi
        mesta--;
        automobil.parkira(rbr);
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada auto izlazi sa parkinga (nakon sto je bio parkiran).
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada auto napusta parking mesto.
    void izadji(int rbr) {
        unique_lock<mutex> l(m);
        // Mesto se oslobađa
        mesta--;
        automobil.napusta(rbr);
        cv.notify_one();
    }
};

#endif // PARKING_H_INCLUDED

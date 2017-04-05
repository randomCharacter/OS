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
    bool slobodno;
public:
    Parking(Automobil& a) : automobil(a) {
        slobodno = true;
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kako bi automobil pokusao da se parkira.
    // Ako je parking zauzet, izvrsenje ove metode ce trajati dok se parking ne oslobodi i auto se parkira.
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.ceka kada je parking zauzet i auto mora da ceka.
    // Potrebno je pozvati metodu automobil.parkira kada auto uspe da se parkira.
    void udji(int rbr) {
        unique_lock<mutex> l(m);
        // Ukoliko nema praznih mesta
        while (!slobodno) {
            automobil.ceka(rbr);
            // Čeka dok se mesto ne oslobodi
            cv.wait(l);
        }
        // Kad se oslobodi
        slobodno = false;
        automobil.parkira(rbr);
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada auto izlazi sa parkinga (nakon sto je bio parkiran).
    //
    // rbr - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada auto napusta parking mesto.
    void izadji(int rbr) {
        unique_lock<mutex> l(m);
        // Izlazak sa parkinga
        slobodno = true;
        // Obaveštava drugu nit da se parking oslobodion
        cv.notify_one();

    }
};

#endif // PARKING_H_INCLUDED

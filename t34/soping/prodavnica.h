#ifndef PRODAVNICA_H_INCLUDED
#define PRODAVNICA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "povratna.h"
#include "kupac.h"

using namespace std;

class Prodavnica {
private:
    Kupac& kupac;
    mutex m;
    condition_variable cv;
    int slobodno;
public:
    Prodavnica(Kupac& k, int n): kupac(k) {
        slobodno = n;
    }

    // Metoda koju poziva nit koja simulira kupca, kada kupac obavlja soping (procedura je opisana u zaglavlju main.cpp)
    // Metoda je blokirajuca - ako kupac mora da ceka da se oslobodi kabina za probu, izvrsenje ce blokirati dok se za to ne steknu uslovi.
    //
    // rbr - Redni broj kupca
    //
    // Potrebno je pozvati metodu kupac.ceka kada su kabine zauzete i kupac mora da ceka.
    // Potrebno je pozvati metodu kupac.proba_odecu kada kupac udje u kabinu da proba odecu.
    // Potrebno je pozvati metodu kupac.zavrsio kada je kupac zavrsio probu odece.
    povratna_vrednost kupi(int rbr) {
        system_clock::time_point poceo = system_clock::now();
        unique_lock<mutex> l(m);
        // Ako nema slobodnih
        while (slobodno == 0) {
            kupac.ceka(rbr);
            cv.wait(l);
        }
        // Ako ima slobodnih
        slobodno--;
        kupac.proba_odecu(rbr);
        // Kad završi sa probanjem
        system_clock::time_point zavrsio = system_clock::now();
        slobodno++;
        povratna_vrednost pv;
        pv.kupio = rand() % 2;
        pv.cekao_na_kabinu = zavrsio - poceo;
        kupac.zavrsio(rbr, pv);

        return pv;
    }
};

#endif // PRODAVNICA_H_INCLUDED

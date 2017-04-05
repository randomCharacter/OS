#ifndef PARKING_H_INCLUDED
#define PARKING_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "automobil.h"

using namespace std;

class Parking {
    Automobil& automobil;
    mutex m;
    condition_variable cv[3];
    // Sledeća kapija sa koje treba pustiti
    int i;
    bool slobodan;

public:
    Parking(Automobil& a) : automobil(a) {
        i = 0;
        slobodan = true;
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada on pokusava da udje na parking.
    // Metoda je blokirajuca - ukoliko nema slobodnog mesta, izvrsenje je u blokadi dok se ono ne oslobodi.
    //
    // rbr  - Redni broj automobila
    // ulaz - Ulaz na koji automobil pokusava da udje
    //
    // Potrebno je pozvati metodu automobil.ceka kada automobil ne moze da pridje parkingu i mora da ceka.
    // Potrebno je pozvati metodu automobil.zauzima kada automobil zauzme parking mesto.
    void udji(int rbr, int ulaz) {
        unique_lock<mutex> l(m);
        // Ukoliko parking nije slobodan ili data kapija nije na redu
        while (!slobodan || ulaz != i) {
            // Automobil čeka
            automobil.ceka(rbr, ulaz);
            cv[ulaz].wait(l);
        }
        // Kad se oslobpdi mesto, ulazi
        slobodan = false;
        automobil.zauzima(rbr);
    }

    // Metoda koju poziva nit koja simulira kretanje automobila kada on napusta parking na koji je prethodno usao.
    //
    // rbr  - Redni broj automobila
    //
    // Potrebno je pozvati metodu automobil.napusta kada automobil napusti parking mesto.
    void izadji(int rbr) {
        unique_lock<mutex> l(m);
        // Automobil napušta parking
        automobil.napusta(rbr);
        slobodan = true;
        // Sledeća kapija se otvara
        i = (++i) % 3;
        cv[i].notify_one();
    }
};

#endif // PARKING_H_INCLUDED

#ifndef TRZNI_CENTAR_H_INCLUDED
#define TRZNI_CENTAR_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <cmath>

#include "kupac.h"

using namespace std;
using namespace chrono;

class Trzni_centar {
private:
    Kupac& kupac;
    mutex m;
    condition_variable cv[2];
    int red[2];
    bool slobodan[2];
public:
    Trzni_centar(Kupac& k) : kupac(k) {
        for (int i = 0; i < 2; i++) {
            slobodan[i] = true;
            red[i] = 0;
        }
    }

    // Metoda koju poziva nit koja simulira kupca kako bi se obavila kupovina.
    // Metoda je blokirajuca - ako su sve kase zauzete, onda se izvrsenje blokira dok se neka ne oslobodi.
    //
    // rbr           - Redni broj kupca
    // broj_artikala - Broj artikala koje kupac zeli da kupi
    //
    // Potrebno je pozvati metodu kupac.ceka kada je izabrana kasa zauzeta i kupac mora da ceka.
    // Potrebno je pozvati metodu kupac.kupuje kada se kasa oslobodi i kupac pocinje kupovinu.
    // Potrebno je pozvati metodu kupac.zavrsio kada je kupac zavrsio kupovinu.
    int kupi(int rbr, int broj_artikala) {
        int kasa;
        {
            unique_lock<mutex> l(m);
            // Ide na onu kasu gde je manji red
            int m = min(red[0], red[1]);
            if (m == red[0]) {
                kasa = 0;
            } else {
                kasa = 1;
            }
            red[kasa]++;
            // Ukoliko je kasa zauzeta
            while (!slobodan[kasa]) {
                // Kupac čeka
                kupac.ceka(rbr, kasa);
                cv[kasa].wait(l);
            }
            // Kada se kasa oslobodi
            slobodan[kasa] = false;
            kupac.kupuje(rbr, kasa, broj_artikala);
        }
        this_thread::sleep_for(seconds(broj_artikala));
        // Nakon što završi kupovinu
        slobodan[kasa] = true;
        red[kasa]--;
        kupac.zavrsio(rbr);
        cv[kasa].notify_one();
    }
};


#endif // TRZNI_CENTAR_H_INCLUDED

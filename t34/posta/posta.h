#ifndef POSTA_H_INCLUDED
#define POSTA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "povratna.h"
#include "klijent.h"

using namespace std;
using namespace chrono;

class Posta {
private:
    Klijent& klijent;
    mutex m;
    condition_variable cv;
    bool slobodan[2];

public:
    Posta(Klijent& kl) : klijent(kl) {
        for (int i = 0; i < 2; i++) {
            slobodan[i] = true;
        }
    }

    // Metoda koju poziva nit koja simulira klijenta kada on uplacuje neki iznos.
    // Metoda je blokirajuca - ako je salter zauzet, metoda ce stajati blokirana dok se on ne oslobodi.
    //
    // rbr   - Redni broj klijenta
    // svota - Svota koju klijent zeli da uplati
    //
    // Potrebno je pozvati metodu klijent.ceka kada su salteri zauzeti i klijent mora da ceka.
    // Potrebno je pozvati metodu klijent.uplacuje kada klijent stupi na salter i vrsi svoje placanje.
    // Potrebno je pozvati metodu klijent.napusta kada klijent zavrsi placanje i napusta salter.
    void uplati(int rbr, int svota) {
        int r;
        {
            unique_lock<mutex> l(m);
            // Ukoliko red nije prazan
            while (!slobodan[0] && !slobodan[1]) {
                klijent.ceka(rbr, svota);
                cv.wait(l);
            }
            // Kada se oslobodi, traži se slobodan
            for (int i = 0; i < 2; i++) {
                if (slobodan[i]) {
                    r = i;
                    break;
                }
            }
            slobodan[r] = false;
            klijent.uplacuje(rbr, r, svota);
        }
        this_thread::sleep_for(seconds(svota));
        unique_lock<mutex> l(m);
        // Izlaz iz reda
        klijent.napusta(rbr, r, svota);
        slobodan[r] = true;
        // Obaveštava drugu nit da ima slobodnih mesta
        cv.notify_one();
    }
};

#endif // POSTA_H_INCLUDED

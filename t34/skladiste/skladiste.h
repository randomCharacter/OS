#ifndef SKLADISTE_H_INCLUDED
#define SKLADISTE_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "kamion.h"

using namespace std;

class Skladiste {
private:
    Kamion& kamion;
    mutex m;
    condition_variable cvZapaljivo, cvObicno;
    int slobodno;
    int zapaljivih;

public:
    Skladiste(Kamion& k) : kamion(k) {
        slobodno = 2;
        zapaljivih = 0;
    }

    // Metoda koju poziva nit koja simulira kretanje kamiona kada on pokusava da istovari robu.
    // Metoda je blokirajuca - ako su sve rampe za istovar zauzete, izvrsenje ce blokirati dok se neka ne oslobodi.
    //
    // rbr       - Redni broj kamiona
    // kolicina  - Kolicina robe koja se prevozi (u tonama)
    // zapaljivo - Ukazuje na to da li je roba koja se prevozi zapaljiva (takva roba ima prioritet pri istovaru!)
    //
    // Potrebno je pozvati metodu kamion.ceka kada su rampe zauzete i kamion mora da ceka.
    // Potrebno je pozvati metodu kamion.istovara kada zapocne istovar robe iz kamiona.
    // Potrebno je pozvati metodu kamion.odlazi kada je kamion zavrsio istovar i odlazi.
    void istovari(int rbr, int kolicina, bool zapaljivo) {
        // Ulazi samo ako je količina robe manja od 7
        if (kolicina <= 7) {
            {
                unique_lock<mutex> l(m);
                // Ako ne može proći
                while (slobodno == 0) {
                    kamion.ceka(rbr, kolicina, zapaljivo);
                    if (zapaljivo) {
                        zapaljivih++;
                        cvZapaljivo.wait(l);
                    } else {
                        cvObicno.wait(l);
                    }
                }
                // Ako može
                kamion.istovara(rbr, kolicina, zapaljivo, slobodno--);
            }
            this_thread::sleep_for(chrono::seconds(kolicina));

            unique_lock<mutex> l(m);
            // Kada završi
            kamion.odlazi(rbr);
            slobodno++;
            // Ako je bio zapaljiv broj se smanjuje
            if (zapaljivo) {
                zapaljivih--;
            }
            // Prvo se puštaju zapaljivi
            if (zapaljivih > 0) {
                cvZapaljivo.notify_one();
            } else {
                cvObicno.notify_one();
            }
        }
    }
};

#endif // SKLADISTE_H_INCLUDED

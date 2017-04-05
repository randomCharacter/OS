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

public:
    Skladiste(Kamion& k) : kamion(k) {
        slobodno = 2;
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
        if (kolicina <= 7) {
            /*if (zapaljivo) {
                // Ako ne može proći
                while (!slobodno) {
                    kamion.ceka(rbr, kolicina, zapaljivo);
                    cvZapaljivo.wait(l);
                }
            } else {
                // Ako ne može proći
                while (!slobodno) {
                    kamion.ceka(rbr, kolicina, zapaljivo);

                }
            }*/
            {
                unique_lock<mutex> l(m);
                // Ako ne može proći
                while (slobodno == 0) {
                    kamion.ceka(rbr, kolicina, zapaljivo);
                    if (zapaljivo) {
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
            // Prvo se puštaju zapaljivi
            cvZapaljivo.notify_one();
            cvObicno.notify_one();

        }
    }
};

#endif // SKLADISTE_H_INCLUDED

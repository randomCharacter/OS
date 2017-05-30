#ifndef PROCESOR_H_INCLUDED
#define PROCESOR_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

class Procesor {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    bool slobodan;
    bool prekid;
    condition_variable cv_proc, cv_prekid;

public:
    Procesor(Dijagnostika& d) : dijagnostika(d) {
        slobodan = true;
        prekid = false;
    }

    // Pomoćna metoda koja vraća referencu na objekat Dijagnostika asociran sa ovim objektom
    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira proces, kako bi se izvršile sve naredbe procesa.
    //
    // id           - id procesa
    // broj_naredbi - ukupan broj naredbi koje ovaj proces izvršava
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Kada proces uspe da zauzme procesor, pre izvršenja naredbe treba da se pozove dijagnostika.proces_izvrsava.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.proces_zavrsio.
    void izvrsi_proces(int id, int broj_naredbi) {
        unique_lock<mutex> l(m);
        // Za svaku naredbu
        for (int i = 0; i < broj_naredbi; i++) {
            // Ukoliko je procesor zauzet
            while (!slobodan) {
                // Šalje se na čekanje
                dijagnostika.proces_ceka(id);
                cv_proc.wait(l);
            }
            // Početak izvršavanja
            slobodan = false;
            dijagnostika.proces_izvrsava(id, i);
            // Izvršavanje
            l.unlock();
            this_thread::sleep_for(seconds(1));
            l.lock();
            // Kraj izvršavanja
            dijagnostika.proces_zavrsio(id, i);
            slobodan = true;
            // Ukoliko se desio prekid odrađuje se, ukoliko ne sledeći proces dolazi na red
            if (prekid) {
                cv_prekid.notify_one();
            } else {
                cv_proc.notify_one();
            }
            // Cooldown perion
            l.unlock();
            this_thread::sleep_for(milliseconds(100));
            l.lock();
        }
    }

    // Metoda koju poziva nit koja simulira obrađivač U/I prekida kako bi se inicirao i obradio prekid.
    //
    // Ukoliko je procesor već zauzet i ne može se obaviti obrađivanje prekida, potrebno je pozvati metodu dijagnostika.obradjivac_ceka a nakon toga obrađivač treba da pređe u stanje čekanja, dok se procesor ne oslobodi.
    // Kada obrađivač uspe da zauzme procesor, pre obrade prekida treba da se pozove dijagnostika.obradjivac_izvrsava.
    // Nakon što je obrađivač završio obradu, potrebno je pozvati dijagnostika.obradjivac_zavrsio.
    void prekini() {
        unique_lock<mutex> l(m);
        prekid = true;
        // Ukoliko je procesor zauzet
        while (!slobodan) {
            // Šalje se na čekanje
            dijagnostika.obradjivac_ceka();
            cv_prekid.wait(l);
        }
        // Početak izvršavanja
        slobodan = false;
        dijagnostika.obradjivac_izvrsava();
        // Izvršavanje
        l.unlock();
        this_thread::sleep_for(milliseconds(300));
        l.lock();
        // Kraj izvršavanja
        dijagnostika.obradjivac_zavrsio();
        slobodan = true;
        // Završavanje i preključivanje na procese
        prekid = false;
        cv_proc.notify_one();
    }
};

#endif // PROCESOR_H_INCLUDED

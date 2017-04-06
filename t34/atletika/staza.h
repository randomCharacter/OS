#ifndef STAZA_H_INCLUDED
#define STAZA_H_INCLUDED

#include <thread>
#include <mutex>
#include <condition_variable>

#include "povratna.h"
#include "takmicar.h"

using namespace std;
using namespace chrono;

class Atletska_staza {
private:
    Takmicar& takmicar;
    mutex m;
    // cv[0] - skakači
    // cv[1] - bacači
    condition_variable cv[2];
    int skakaca;
    int bacaca;
    bool slobodna;

public:
    Atletska_staza(Takmicar& tak) : takmicar(tak) {
        slobodna = true;
        skakaca = 0;
        bacaca = 0;
    }

    // Metoda koju poziva nit koja simulira skakaca kako bi takmicar obavio skok.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.skakac_ceka kada skakac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.skakac_skace kada skakac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.skakac_zavrsio kada je skakac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost skaci(int rbr) {
        // Beleženje početka
        system_clock::time_point pocetak = system_clock::now();
        {
            unique_lock<mutex> l(m);
            // Broj skakača se povećava
            skakaca++;

            // Ukoliko je staza zauzeta
            while (!slobodna) {
                // Skakač čeka
                takmicar.skakac_ceka(rbr);
                cv[1].wait(l);
            }
            // Kada se oslobodi počinje sa skakanjem
            slobodna = false;
            takmicar.skakac_skace(rbr);
        }
        this_thread::sleep_for(chrono::seconds(1));
        unique_lock<mutex> l(m);
        // Beleženje kraja
        system_clock::time_point kraj = system_clock::now();
        // Beleženje rezultata
        Povratna_vrednost ret;
        ret.rezultat = rand() % 10;
        ret.trajanje = kraj - pocetak;
        takmicar.skakac_zavrsio(rbr, ret);
        // Staza se oslobađa i o tome se obaveštavaju druge niti
        slobodna = true;
        // Ako ima bacača obaveštaavaju se da je staza prazna
        if (bacaca > 0) {
            cv[0].notify_one();
        // Inače se obaveštava sledeći skakač
        } else {
            cv[1].notify_one();
        }
        // Broj skakača se smanjuje
        skakaca--;

        return ret;
    }

    // Metoda koju poziva nit koja simulira bacaca kako bi takmicar obavio bacanje.
    // Ukoliko je zaletiste zauzeto, ova metoda ce stajati blokirana dok se ono ne oslobodi i takmicar obavi nastup.
    //
    // rbr - Redni broj takmicara
    //
    // Potrebno je pozvati metodu takmicar.bacac_ceka kada bacac ne moze da stupi na zaletiste.
    // Potrebno je pozvati metodu takmicar.bacac_skace kada bacac stupi na zaletiste, pre skoka.
    // Potrebno je pozvati metodu takmicar.bacac_zavrsio kada je bacac zavrsio i kada treba prikazati njegov rezultat.
    Povratna_vrednost baciKoplje(int rbr) {
        // Beleženje početka
        system_clock::time_point pocetak = system_clock::now();
        {
            unique_lock<mutex> l(m);
            // Broj bacača se povećava
            bacaca++;

            // Ukoliko je staza zauzeta
            while (!slobodna) {
                // Bacač čeka
                takmicar.bacac_ceka(rbr);
                cv[0].wait(l);
            }
            // Kada se oslobodi počinje sa skakanjem
            slobodna = false;
            takmicar.bacac_baca(rbr);

        }
        this_thread::sleep_for(chrono::seconds(2));
        unique_lock<mutex> l(m);
        // Beleženje kraja
        system_clock::time_point kraj = system_clock::now();
        // Beleženje rezultata
        Povratna_vrednost ret;
        ret.rezultat = rand() % 101;
        ret.trajanje = kraj - pocetak;
        takmicar.bacac_zavrsio(rbr, ret);
        // Staza se oslobađa i o tome se obaveštavaju druge niti
        slobodna = true;
        // Ako ima skakača obaveštaavaju se da je staza prazna
        if (skakaca > 0) {
            cv[1].notify_one();
        // Inače se obaveštava sledeći skakač
        } else {
            cv[0].notify_one();
        }
        // Broj bacača se smanjuje
        bacaca--;

        return ret;
    }
};

#endif // STAZA_H_INCLUDED

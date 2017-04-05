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
    condition_variable cv;
    bool slobodna;

public:
    Atletska_staza(Takmicar& tak) : takmicar(tak) {
        slobodna = true;
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

            // Ukoliko je staza zauzeta
            while (!slobodna) {
                // Skakač čeka
                takmicar.skakac_ceka(rbr);
                cv.wait(l);
            }
            // Kada se oslobodi počinje sa skakanjem
            slobodna = false;
            takmicar.skakac_skace(rbr);
        }
        this_thread::sleep_for(chrono::seconds(1));
        // Beleženje kraja
        system_clock::time_point kraj = system_clock::now();
        // Beleženje rezultata
        Povratna_vrednost ret;
        ret.rezultat = rand() % 10;
        ret.trajanje = kraj - pocetak;
        takmicar.skakac_zavrsio(rbr, ret);
        // Staza se oslobađa i o tome se obaveštavaju druge niti
        slobodna = true;
        cv.notify_one();

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

            // Ukoliko je staza zauzeta
            while (!slobodna) {
                // Bacač čeka
                takmicar.bacac_ceka(rbr);
                cv.wait(l);
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
        cv.notify_one();

        return ret;
    }
};

#endif // STAZA_H_INCLUDED

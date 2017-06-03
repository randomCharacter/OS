#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include <mutex>

#include "dijagnostika.h"
#include "cv_hrono.h"
#include "red.h"

using namespace std;
using namespace chrono;

class Rasporedjivac {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    int aktivan;
    vector<Red> redovi;

public:
    Rasporedjivac(Dijagnostika& d, int broj_nivoa_prioriteta) : dijagnostika(d) {
        aktivan = -1;
        for (int i = 0; i < broj_nivoa_prioriteta; i++) {
            redovi.push_back(Red(i));
        }
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa   - ID procesa
    // broj_naredbi - ukupan broj naredbi koje proces treba da izvrši
    // prioritet    - prioritet procesa
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
	void izvrsi(int id_procesa, int broj_naredbi, int prioritet) {
        unique_lock<mutex> l(m);
        for (int i = 0; i < broj_naredbi; i++) {
            // Ukoliko je procesor zauzet
            while (aktivan != id_procesa && aktivan != -1) {
                dijagnostika.proces_ceka(id_procesa);
                redovi[prioritet].dodaj_u_red(id_procesa, l);
            }
            aktivan = id_procesa;
            // Kada dođe na red
            l.unlock();
            this_thread::sleep_for(milliseconds(300));
            l.lock();
            // Traži najprioritetniji spreman proces
            int spreman = -1;
            for (auto it = redovi.begin(); it != redovi.end(); it++) {
                // Ukoliko ima spremnih procesa u redu
                if (!it->prazan()) {
                    spreman = it->preuzmi_prioritet();
                    break;
                }
            }
            // Ukoliko je našao red
            if (spreman != -1) {
                // Ukoliko nađeni proces ima veći (ili jednak) prioritet od trenutnog ili ukoliko trenutni nema više naredbi
                if (spreman < prioritet || broj_naredbi-1 == i) {
                    aktivan = redovi[spreman].izbaci_iz_reda();
                }
            // Ukoliko nema više procesa
            } else {
                aktivan = -1;
            }
            dijagnostika.izvrsio_naredbu(id_procesa, i);
        }
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED

#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <vector>

#include "dijagnostika.h"
#include "red.h"

using namespace std;

class Rasporedjivac {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    int aktivan;
    vector<Red> redovi;
    int broj_prioriteta;

public:
	Rasporedjivac(Dijagnostika& d, int broj_nivoa_prioriteta) : dijagnostika(d), broj_prioriteta(broj_nivoa_prioriteta) {
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
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
	void izvrsi(int id_procesa, int broj_naredbi) {
        unique_lock<mutex> l(m);
        int prioritet = 0; // na početku ima najviši prioritet
        for (int i = 0; i < broj_naredbi; i++) {
            // Ukoliko je procesor zauzet
            while (aktivan != id_procesa && aktivan != -1) {
                dijagnostika.proces_ceka(id_procesa);
                redovi[prioritet].dodaj_u_red(id_procesa, l);
            }
            // Kada se oslobodi
            aktivan = id_procesa;
            // Izvršavanje
            l.unlock();
            this_thread::sleep_for(milliseconds(300));
            l.lock();
            // Smanjuje se prioritet ako je to moguće
            if (prioritet < broj_prioriteta) {
                prioritet++;
            }
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
            dijagnostika.izvrsio_naredbu(id_procesa, i, prioritet);
        }
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED

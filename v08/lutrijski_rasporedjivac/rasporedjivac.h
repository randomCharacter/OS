#ifndef RASPOREDJIVAC_H_INCLUDED
#define RASPOREDJIVAC_H_INCLUDED

#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"
#include "cv_tag.h"

using namespace std;
using namespace chrono;

struct Proces {
	int id;
	int broj_naredbi;
	vector<int> tiketi;
	Proces(int i, int b, vector<int> t) : id(i), broj_naredbi(b), tiketi(t) {}
};

class Rasporedjivac {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    cv_tag cv;
    int aktivni;
    map<int, int> tiketi;

public:
    Rasporedjivac(Dijagnostika& d) : dijagnostika(d) {
        aktivni = -1;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // p - struktura koja sadrži ID procesa, broj naredbi koje treba da izvrši kao i sve tikete koji pripadaju tom procesu.
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što je proces izvršio naredbu, potrebno je pozvati dijagnostika.izvrsio_naredbu.
	void izvrsi(Proces p) {
        unique_lock<mutex> l(m);
        // Mapiranje svih tiketa na dati proces
        for (auto it = p.tiketi.begin(); it != p.tiketi.end(); it++) {
            tiketi[*it] = p.id;
        }

        for (int i = 0; i < p.broj_naredbi; i++) {
            // Ukoliko procesor pripada drugom procesu
            while (aktivni != p.id && aktivni != -1) {
                dijagnostika.proces_ceka(p.id);
                cv.wait(l, p.id);
            }
            // Kada dođe na red
            aktivni = p.id;
            // Izvrši se
            l.unlock();
            this_thread::sleep_for(milliseconds(300));
            l.lock();
            // Ukoliko se proces završio
            if (i == p.broj_naredbi - 1) {
                // Izbacuju se svi njegovi tiketi iz mape
                for (auto it = p.tiketi.begin(); it != p.tiketi.end(); it++) {
                    tiketi.erase(tiketi.find(*it));
                }
            }
            // Biranje sledećeg procesa na osnovu izvlačenja tiketa
            if (!tiketi.empty()) {
                // Biranje random tiketa (rednog broja)
                int sledeci_t = rand() % tiketi.size();
                // Nalaženje istog u listi
                auto it = tiketi.begin();
                // Dolaženje do datog tiketa u mapi
                for (int j = 0; j < sledeci_t; j++, it++);
                // Uzimanje id procesa iz izabranog tiketa
                aktivni = it->second;
                // Buđenje procesa čiji je tiket izabran
                cv.notify(aktivni);
            // Ukoliko nema tiketa
            } else {
                aktivni = -1;
            }
            dijagnostika.izvrsio_naredbu(p.id, i, aktivni);
        }
	}
};

#endif // RASPOREDJIVAC_H_INCLUDED

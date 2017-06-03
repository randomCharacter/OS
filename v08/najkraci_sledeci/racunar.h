#ifndef RACUNAR_H_INCLUDED
#define RACUNAR_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <vector>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

struct proces {
    int id_procesa;
    int trajanje;
    condition_variable cv;
    proces(int id, int t) : id_procesa(id), trajanje(t) { };
};

class Racunar {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    vector<proces*> procesi;
    condition_variable cv_ras;
    bool slobodan, rasp;
    int aktivan;

public:
    Racunar(Dijagnostika& d) : dijagnostika(d) {
        aktivan = -1;
        rasp = false;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se proces izvršio
    //
    // id_procesa   - ID procesa
    // trajanje     - trajanje procesa (izraženo u stotinama milisekundi)
    //
    // Ukoliko je procesor već zauzet i ne mogu se izvršavati naredbe procesa, potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da pređe u stanje čekanja.
    // Nakon što se proces izvršio, potrebno je pozvati dijagnostika.proces_zavrsio.
    void izvrsi(int id_procesa, int trajanje) {
        unique_lock<mutex> l(m);
        // Kreiranje novog procesa
        proces *p = new proces(id_procesa, trajanje);
        procesi.push_back(p);
        while (aktivan != id_procesa) {
            dijagnostika.proces_ceka(id_procesa);
            cv_ras.notify_one();
            p->cv.wait(l);
        }
        // Izvršavanje
        l.unlock();
        this_thread::sleep_for(milliseconds(trajanje * 100));
        l.lock();
        // Završavanje
        dijagnostika.proces_zavrsio(id_procesa);
        aktivan = -1;
        cv_ras.notify_one();
        delete p;
    }

    // Metoda koju poziva nit koja simulira raspoređivač operativnog sistema, kako bi se izabrao sledeći proces za izvršenje.
    //
    // Ukoliko još uvek ne postoji potreba da se raspoređivač aktivira, potrebno je pozvati metodu dijagnostika.rasporedjivac_ceka a nakon toga nit treba da pređe u stanje čekanja.
    // Nakon što se raspoređivač aktivirao i izabrao sledeći proces, potrebno je pozvati dijagnostika.rasporedjivac_izvrsio.
    void rasporedjuj() {
        while (true) {
            unique_lock<mutex> l(m);
            // Ukoliko je vektor procesa prazan
            while (aktivan != -1 || procesi.empty()) {
                cv_ras.wait(l);
            }
            // Ukoliko ima procesa
            int min = procesi[0]->trajanje;
            auto proc = procesi.begin();
            // Traženje najkraćeg
            for (auto it = procesi.begin(); it != procesi.end(); it++) {
                if ((*it)->trajanje < min) {
                    min = (*it)->trajanje;
                    proc = it;
                }
            }
            // Pokretanje procesa
            aktivan = (*proc)->id_procesa;
            (*proc)->cv.notify_one();
            // Izbacivanje procesa iz liste
            procesi.erase(proc);
        }
    }
};

#endif // RACUNAR_H_INCLUDED

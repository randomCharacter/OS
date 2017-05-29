#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <queue>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

// Struktura zahteva koja sadrži sve potrebne informacije za kreiranje istog
typedef struct zahtev_st{
    condition_variable cv;
    int id_procesa;
    int broj_staze;
    bool gotov;
    zahtev_st(int id, int br) : id_procesa(id), broj_staze(br) {
        gotov = false;
    }
} zahtev;

class OS {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    condition_variable cv_p, cv_z;
    queue<zahtev*> zahtevi; // Red za zahteve

public:
    OS(Dijagnostika& d) : dijagnostika(d) {

    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa, kako bi se uputio zahtev ka U/I uređaju
    //
    // id_procesa - ID procesa koji upućuje zahtev
    // broj_staze - broj staze diska kojoj nit želi da pristupi
    //
    // Metoda treba da formira novi zahtev i uputi ga U/I uređaju na obradu. Proces će biti blokiran dok god se ovaj zahtev ne izvrši. Pre nego što stupi u blokadu, potrebno je pozvati dijagnostika.proces_ceka.
    void uputi_UI_zahtev(int id_procesa, int broj_staze) {
        unique_lock<mutex> l(m);
        // Pravi novi zahtev i dodaje u red
        zahtev* z = new zahtev(id_procesa, broj_staze);
        zahtevi.push(z);
        // Obaveštava obrađivač da ima zahteva
        cv_z.notify_one();
        // Završava i briše zahtev
        while (!z->gotov) {
            z->cv.wait(l);
        }
        delete z;
    }

    // Metoda koju poziva nit koja simulira deo operativnog sistema koji upravlja diskom, kako bi se obradio jedan pristigli zahtev
    //
    // Prema uslovima zadatka, metoda treba da obradi zahtev koji je najranije pristigao.
    //
    // Povratna vrednost metode treba da bude broj staze koji je obrađen.
    int obradi_zahtev() {
        unique_lock<mutex> l(m);
        // Ukoliko je red prazan čeka
        while (zahtevi.empty()) {
            cv_z.wait(l);
        }
        // Vadi zahtev iz reda
        zahtev* z = zahtevi.front();
        zahtevi.pop();
        // Izvršava zahtev
        l.unlock();
        this_thread::sleep_for(milliseconds(300));
        l.lock();
        // Završava zahtev
        z->gotov = true;
        z->cv.notify_one();
        return z->broj_staze;
    }
};

#endif // OS_H_INCLUDED

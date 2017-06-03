#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"
#include "naredba.h"
#include "povratna_vrednost.h"

using namespace std;
using namespace chrono;

class Program {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    condition_variable cv_r, cv_v;
    int slobodna, virtualna; // Redom slobodan RAM i zauzeta virtualna memorija
    bool zauzmi_vm; // Indikator zauzimanja virtualne memorije
    int kolicina;

    void zauzmi(int koliko) {
        unique_lock<mutex> l(m);
        // Ako ima dovoljno memorije
        if (slobodna >= koliko) {
            // Zauzima ram
            slobodna -= koliko;
        } else {
            // Pozove se zauzimanje virtuelne
            zauzmi_vm = true;
            kolicina = koliko;
            cv_v.notify_one();
            // Čekanje dok se virtuelna ne zauzme
            while (zauzmi_vm) {
                cv_r.wait(l);
            }
        }
        this_thread::sleep_for(seconds(1)); // lock i unlock ne smeju da postoje
    }

public:
    Program(Dijagnostika& d, int kapacitet) : dijagnostika(d), slobodna(kapacitet) {
        virtualna = 0;
        kolicina = 0;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje programa, kako bi zauzela potrebnu memoriju
    //
    // naredba - naredba koja se izvršava (naredba kojom se zauzima memorija)
    // Vraća instancu klase Povratna_vrednost koja sadrži opis zauzeća memorije NAKON izvršenja naredbe.
    Povratna_vrednost izvrsi_naredbu(Naredba naredba) {
        // Provera tipa naredbi
        if (naredba.tip == "repeat") {
            for (int i = 0; i < naredba.ponavljanja; i++) {
                zauzmi(naredba.kolicina_memorije);
            }
        } else {
            zauzmi(naredba.kolicina_memorije);
        }
        // Vraćanje vrednosi
        return {slobodna, virtualna};
    }

    // Metoda koju poziva nit koja simulira deo operativnog sistema koji se bavi zauzimanjem virtuelne memorije kako bi se zauzela određena količina VM
    // Potrebnu količinu VM (kao i bilo koju drugu neophodnu informaciju) preneti u nekom izabranom polju klase.
    int zauzmi_virtuelnu_memoriju() {
        unique_lock<mutex> l(m);
        // Ukoliko nije potrebno zauzeti
        while (!zauzmi_vm) {
            // Ide na čekanje
            cv_v.wait(l);
        }
        // Zauzimanje
        this_thread::sleep_for(seconds(1)); // lock i unlock ne smeju da postoje
        virtualna += kolicina;
        zauzmi_vm = false;
        // Završavanje i obaveštavanje drugih niti
        cv_r.notify_one();
        return virtualna;
    }
};

#endif // PROGRAM_H_INCLUDED

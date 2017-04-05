#ifndef BANKA_H_INCLUDED
#define BANKA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "valuta.h"
#include "kredit.h"

using namespace std;

class Banka {
private:
    Kredit& kredit;
    mutex m;
    condition_variable cvd, cve;
    int dsaldo, esaldo;
public:

    Banka(Kredit& kr, int inicijalni_dsaldo, int inicijalni_esaldo)
        : kredit(kr) {
        dsaldo = inicijalni_dsaldo;
        esaldo = inicijalni_esaldo;
    }

    // Metoda koju poziva nit koja simulira klijenta banke, kada klijent zatrazi kredit od banke.
    // Ukoliko banka nema odgovarajuca sredstva kako bi omogucila kredit, metoda ce blokirati dok se ta sredstva ne oslobode.
    //
    // rbr - Redni broj klijenta
    // svota - Novcani iznos koji je potreban klijentu
    // valuta - Valuta u kojem je novcani iznos
    //
    // Potrebno je pozvati metodu kredit.ceka kada nema sredstava odgovarajuce valute da se odobri kredit.
    // Potrebno je pozvati metodu kredit.dobio kada se kredit realizuje.
    void uzmi_kredit(int rbr, int svota, Valuta valuta) {
        if (valuta == DINAR) {
            unique_lock<mutex> l(m);
            // Ukoliko nema dovoljno sredstava
            while (dsaldo < svota) {
                kredit.ceka(rbr, svota, valuta);
                // Čeka dok se slov ne ispuni
                cvd.wait(l);
            }
            // Kada se pojavi dovoljno sredstava
            dsaldo -= svota;
            kredit.dobio(rbr, svota, dsaldo, valuta);

        } else {
            unique_lock<mutex> l(m);
            // Ukoliko nema dovoljno sredstava
            while (esaldo < svota) {
                kredit.ceka(rbr, svota, valuta);
                // Čeka dok se slov ne ispuni
                cve.wait(l);
            }
            // Kada se pojavi dovoljno sredstava
            esaldo -= svota;
            kredit.dobio(rbr, svota, esaldo, valuta);
        }
    }

    // Metoda koju poziva nit koja simulira klijenta banke, kada klijent vrati kredit koji je prethodno uzeo od banke.
    //
    // rbr - Redni broj klijenta
    // svota - Novcani iznos koji je potreban klijentu
    // valuta - Valuta u kojem je novcani iznos
    //
    // Potrebno je pozvati metodu kredit.vratio kada je kredit vracen.
    void vrati_kredit(int rbr, int svota, Valuta valuta) {
        if (valuta == DINAR) {
            unique_lock<mutex> l(m);
            // Vraćanje sredstava
            dsaldo += svota;
            kredit.vratio(rbr, svota, dsaldo, valuta);
            // Obaveštava drugu nit da se stanje na računu promenilo
            cvd.notify_one();
        } else {
            unique_lock<mutex> l(m);
            // Vraćanje sredstava
            esaldo += svota;
            kredit.vratio(rbr, svota, esaldo, valuta);
            // Obaveštava drugu nit da se stanje na računu promenilo
            cve.notify_one();
        }
    }
};

#endif // BANKA_H_INCLUDED

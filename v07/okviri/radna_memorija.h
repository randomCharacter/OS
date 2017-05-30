#ifndef RADNA_MEMORIJA_H_INCLUDED
#define RADNA_MEMORIJA_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <vector>

#include "dijagnostika.h"

using namespace std;

class Radna_memorija {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    condition_variable cv;
    int ukupno_okvira;
    int slobodno_okvira;
    vector<int> mem;

public:
    // dijagnostika  - referenca na instancu klase Dijagnostika
	// ukupno_okvira - ukupan broj okvira u memoriji
    Radna_memorija(Dijagnostika& d, int ukupno_okvira) : dijagnostika(d), ukupno_okvira(ukupno_okvira), slobodno_okvira(ukupno_okvira) {
        unique_lock<mutex> l(m);
        mem.resize(ukupno_okvira, -1);
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi pokušala da učita proces u radnu memoriju
    //
    // broj_stranica - ukupan broj stranica koje proces sadrži
    // id_procesa    - identifikator procesa
    //
    // Ukoliko trenutno nema na raspolaganju dovoljno memorije (broj stranica u procesu je veći od broja raspoloživih okvira memorije), potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da čeka dok se memorija ne oslobodi.
    // Kada proces uspe da dođe do potrebne radne memorije, treba da se pozove dijagnostika.proces_izvrsava. Nakon toga,
    // kada proces zauzme okvire radne memorije, potrebno je pozvati dijagnostika.ispisi_okvire kako bi se prikazalo koje je okvire zauzeo.
    void ucitaj(int broj_stranica, int id_procesa) {
        unique_lock<mutex> l(m);
        // Ukoliko nema dovoljno mesta
        if (slobodno_okvira < broj_stranica) {
            dijagnostika.proces_ceka(id_procesa);
        }
        // Odvojeni su kako se ispis čekanja ne bi ponavljao
        while (slobodno_okvira < broj_stranica) {
            cv.wait(l);
        }
        // Kada ima radne memorije
        dijagnostika.proces_se_izvrsava(id_procesa);
        int zauzeo = 0;
        int i = 0;
        // Indeksi okvira koje je zauzeo
        vector<int> zauzeti;
        // Zauzima slobone lokacije dok ih ne zauzme dovoljno
        while (zauzeo < broj_stranica && i < ukupno_okvira) {
            if (mem[i] == -1) {
                mem[i] = id_procesa;
                zauzeti.push_back(i);
                zauzeo++;
            }
            i++;
        }
        // Završava zauzimanje
        slobodno_okvira -= zauzeo;
        dijagnostika.ispisi_okvire(zauzeti.begin(), zauzeti.end());
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi oslobodila radnu memoriju koju je koristila tokom izvršenja
    //
    // id_procesa - identifikator procesa koji oslobađa memoriju
    void oslobodi(int id_procesa) {
        int oslobodio = 0;
        // Prolazi kroz sve lokacije
        for (auto it = mem.begin(); it != mem.end(); it++) {
            // Oslobađa one koje pripadaju datom procesu
            if (*it == id_procesa) {
                *it = -1;
                oslobodio++;
            }
        }
        // Završava oslobađanje i obaveštava ostale
        slobodno_okvira += oslobodio;
        cv.notify_all();
    }
};

#endif // RADNA_MEMORIJA_H_INCLUDED

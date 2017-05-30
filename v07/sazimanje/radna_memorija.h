#ifndef RADNA_MEMORIJA_H_INCLUDED
#define RADNA_MEMORIJA_H_INCLUDED

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

class Radna_memorija {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    condition_variable cv, cvs;
    int ukupno_okvira;
    int slobodno_okvira;
    vector<int> mem;
    bool sazeto;

public:
    Radna_memorija(Dijagnostika& d, int ukupno_lokacija) : dijagnostika(d), ukupno_okvira(ukupno_lokacija), slobodno_okvira(ukupno_lokacija) {
        unique_lock<mutex> l(m);
        mem.resize(ukupno_okvira, -1);
        sazeto = true;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje procesa kako bi proces zauzeo potrebnu memoriju i krenuo sa radom
    //
    // id_procesa          - identifikator procesa
    // br_lokacija_procesa - ukupan broj lokacija memorije koje su neophodne za izvršenje procesa
    // trajanje            - koliko dugo se proces izvršava (u sekundama)
    //
    // Ukoliko trenutno nema na raspolaganju dovoljno slobodne memorije (moraju se zauzeti uzastopni okviri u memoriji!), potrebno je pozvati metodu dijagnostika.proces_ceka a nakon toga proces treba da čeka dok se memorija ne oslobodi.
    // Kada proces uspe da dođe do potrebne radne memorije, treba da se pozove dijagnostika.proces_zauzeo_okvire.
    // Kada se proces završi, potrebno je pozvati metodu dijagnostika.proces_zavrsio.
    // Metodu dijagnostika.ispisi_memoriju potrebno je pozvati u sledećim momentima: nakon što proces zauzme memoriju i nakon što proces oslobodi memoriju.
    void koristi(int id_procesa, int br_lokacija_procesa, int trajanje) {
        unique_lock<mutex> l(m);
        // Ukoliko nema dovoljno okvira proces čeka
        if (slobodno_okvira < br_lokacija_procesa) {
            dijagnostika.proces_ceka(id_procesa);
        }
        // Odvojeno, kako se ne bi stalno ponavljao ispis za čekanje
        while (slobodno_okvira < br_lokacija_procesa || !sazeto) {
            cv.wait(l);
        }
        // Kada se oslobodi
        int zauzeo = 0;
        int i = 0;
        int pocetni = 0;
        // Potraga za prvim slobodnim mestom i zauzimanje
        while (zauzeo < br_lokacija_procesa && i < ukupno_okvira) {
            if (mem[i] == -1) {
                if (pocetni == 0) {
                    pocetni = i;
                }
                mem[i] = id_procesa;
                zauzeo++;
            }
            i++;
        }
        // Kraj zauzimanja
        slobodno_okvira -= zauzeo;
        dijagnostika.proces_zauzeo_okvire(id_procesa, pocetni, pocetni + br_lokacija_procesa);
        dijagnostika.ispisi_memoriju(mem.begin(), mem.end());
        // Simulacija čekanja
        l.unlock();
        this_thread::sleep_for(seconds(trajanje));
        l.lock();
        // Oslobađanje
        int oslobodio = 0;
        for (vector<int>::iterator it = mem.begin(); it != mem.end(); it++) {
            if (*it == id_procesa) {
                *it = -1;
                oslobodio++;
            }
        }
        slobodno_okvira += oslobodio;
        dijagnostika.proces_zavrsio(id_procesa);
        dijagnostika.ispisi_memoriju(mem.begin(), mem.end());
        // Obaveštava proces za sažimanje da ima posla
        sazeto = false;
        cvs.notify_one();

    }

    // Metoda koju poziva nit operativnog sistema koji se bavi sažimanjem memorije
    //
    // Nakon što je sažimanje obavljeno potrebno je pozvati metodu dijagnostika.sazimanje_obavljeno a nakon toga i metodu dijagnostika.ispisi_memoriju kako bi se prikazali efekti sažimanja memorije.
    void sazimanje() {
        unique_lock<mutex> l(m);
        // Ukoliko je već sažeta
        while (sazeto) {
            // Ide na čekanje
            cvs.wait(l);
        }
        // Proces sažimanja
        for (int i = 0; i < ukupno_okvira-1; i++) {
            // Traži prvi deo memorije koji nije ničiji
            if (mem[i] == -1) {
                int j = i;
                int sh = 1;
                // Ide dok ne naleti na prvi zauzeti okvir ili kraj
                while (mem[j] == -1 && j < ukupno_okvira) {
                    j++;
                    sh++;
                }
                // Ukoliko je naišao na okvir zauzet od drugog procesa
                if (j < ukupno_okvira) {
                    // Pomera sve lokacije za onoliko mesta kolika je rupa
                    for (int j = i; j < i+sh; j++) {
                        if (j+sh < ukupno_okvira) {
                            mem[j] = mem[j+sh];
                            mem[j+sh] = -1;
                        }
                    }
                }
            }
        }
        // Obaveštava da je završio sa sažimanjem
        sazeto = true;
        cv.notify_all();
    }
};

#endif // RADNA_MEMORIJA_H_INCLUDED

#ifndef MEMORIJA_H_INCLUDED
#define MEMORIJA_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <vector>

#include "dijagnostika.h"

using namespace std;
using namespace chrono;

enum Stanje {SLOBODAN, CITANJE, UPIS };

// Pomoćna struktura zadužena za regulisanje pristupa svakoj reči
struct pristup {
    Stanje stanje;
    int trenutno_cita;
    int ceka_citanje;
    condition_variable cv_r;
    condition_variable cv_w;
    pristup() {
        stanje = SLOBODAN;
        trenutno_cita = ceka_citanje = 0;
    }
};

class Memorija {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    vector<char> mem;
    vector<pristup*> pristupi; // pristup*, a ne pristup zato sto condition_variable ne može da se kopira

public:
    Memorija(Dijagnostika& d, int bajtova) : dijagnostika(d) {
        mem.resize(bajtova, 0);
        // Broj reči (4 bajta = jedna reč)
        int reci = bajtova / 4 + 1;
        // Ubacivanje reči u vektor
        for (int i = 0; i < reci; i++) {
            pristupi.push_back(new pristup);
        }
    }

    ~Memorija() {
        for (auto it = pristupi.begin(); it != pristupi.end(); it++) {
            delete *it;
        }
    }
    // Metoda koju poziva nit koja simulira proces koji pristupa memoriji kako bi obavila čitanje iz nje
    //
    // rbp    - Redni broj procesa
    // adresa - Lokacija sa koje se čita vrednost prethodno upisana u memoriju
    //
    // Potrebno je pozvati dijagnostika.proces_ceka_citanje kada je memorijska reč zaključana i proces mora da čeka.
    // Potrebno je pozvati dijagnostika.proces_procitao onda kada je ostvaren pristup memoriji i kada je vrednost učitana iz nje.
    char citaj(int rbp, int adresa) {
        unique_lock<mutex> l(m);
        int rbr_reci = adresa / 4;

        // Povećava se broj čitača koji čeka čitanje
        (pristupi[rbr_reci]->ceka_citanje)++;
        // Ukoliko je upis u toku
        while (pristupi[rbr_reci]->stanje == UPIS) {
            dijagnostika.proces_ceka_citanje(rbp, adresa);
            pristupi[rbr_reci]->cv_r.wait(l);
        }
        // Kada dođe na red broj čitača se smanjuje
        (pristupi[rbr_reci]->ceka_citanje)--;
        // Čitanje
        (pristupi[rbr_reci]->trenutno_cita)++;
        pristupi[rbr_reci]->stanje = CITANJE;
        l.unlock();
        this_thread::sleep_for(seconds(1));
        l.lock();
        // Kraj čitanja
        char vrednost = mem[adresa];
        (pristupi[rbr_reci]->trenutno_cita)--;
        // Ukoliko nema više čitača
        if (pristupi[rbr_reci]->trenutno_cita == 0) {
            pristupi[rbr_reci]->stanje = SLOBODAN;
            pristupi[rbr_reci]->cv_w.notify_one();
        }
        dijagnostika.proces_procitao(rbp, adresa, vrednost);
        return vrednost;
    }

    // Metoda koju poziva nit koja simulira proces koji pristupa memoriji kako bi obavila upis u nju
    //
    // rbp      - Redni broj procesa
    // adresa   - Lokacija u koju se upisuje nova vrednost
    // vrednost - Vrednost koja se upisuje u memoriju
    //
    // Potrebno je pozvati dijagnostika.proces_ceka_upis kada je memorijska reč zaključana i proces mora da čeka.
    // Potrebno je pozvati dijagnostika.proces_upisao onda kada je ostvaren pristup memoriji i kada je vrednost upisana u nju.
    void pisi(int rbp, int adresa, char vrednost) {
        unique_lock<mutex> l(m);
        int rbr_reci = adresa / 4;

        // Ukoliko reč nije slobodna
        while (pristupi[rbr_reci]->stanje != SLOBODAN) {
            dijagnostika.proces_ceka_citanje(rbp, adresa);
            pristupi[rbr_reci]->cv_w.wait(l);
        }
        // Kada se oslobodi
        pristupi[rbr_reci]->stanje = UPIS;
        // Pisanje
        pristupi[rbr_reci]->stanje = UPIS;
        l.unlock();
        this_thread::sleep_for(seconds(1));
        l.lock();
        // Kraj čitanja
        mem[adresa] = vrednost;
        pristupi[rbr_reci]->stanje = SLOBODAN;
        dijagnostika.proces_upisao(rbp, adresa, vrednost);
        // Ukoliko ima čitača na čekanju
        if (pristupi[rbr_reci]->ceka_citanje > 0) {
            pristupi[rbr_reci]->cv_r.notify_one();
        } else {
            pristupi[rbr_reci]->cv_w.notify_one();
        }
    }
};

#endif // MEMORIJA_H_INCLUDED

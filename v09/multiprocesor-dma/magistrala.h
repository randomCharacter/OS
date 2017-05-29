#ifndef MAGISTRALA_H_INCLUDED
#define MAGISTRALA_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <queue>

#include "dijagnostika.h"

using namespace std;

// Klasa magistrale (sabirnice). Glavna deljena promenljiva za sinhronizaciju pristupa procesora memoriji i dma kontroleru.
class Magistrala {
private:
    Dijagnostika& dijagnostika;
    Memorija *mem;
    mutex m;
    condition_variable cv_dma, cv;

public:
    // Stanja sabirnice - slobodna, čitanje iz memorije, pisanje u memoriju, dma transfer. Primetiti sličnost sa stanjima iz zadatka "multiprocesor":
    enum Stanje { SLOBODNA, MEM_CITAJ, MEM_PISI, DMA};

    // Struktura u koju se beleže parametri DMA transfera. Odakle, kome i koliko bajtova se čita ili piše.
    struct DMA_transfer {
        int odakle;
        int koliko;
        int kome;
    };

private:
    Stanje stanje;
    queue<DMA_transfer> transferi;

public:

    Magistrala(Dijagnostika& d, Memorija& mem) : dijagnostika(d), mem(&mem) {
        stanje = SLOBODNA;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    char citaj_memoriju(int adresa) {
        unique_lock<mutex> l(m);
        // Ukoliko propusnica nije slobodna čeka
        while (stanje != SLOBODNA) {
            cv.wait(l);
        }
        // Kad se oslobodi
        stanje = MEM_CITAJ;
        // Čekanje za zauzimanje propusnice
        this_thread::sleep_for(milliseconds(700));
        // Oslobađa propusnicu i obaveštava druge procese
        stanje = SLOBODNA;
        cv.notify_one();
        return mem->citaj(adresa);
    }

    void pisi_u_memoriju(int adresa, char vrednost) {
        unique_lock<mutex> l(m);
        // Ukoliko propusnica nije slobodna čeka
        while (stanje != SLOBODNA) {
            cv.wait(l);
        }
        // Kad se oslobodi
        stanje = MEM_PISI;
        // Čekanje za zauzimanje propusnice
        this_thread::sleep_for(milliseconds(700));
        // Oslobađa propusnicu i obaveštava druge procese
        mem->pisi(adresa, vrednost);
        stanje = SLOBODNA;
        cv.notify_one();
    }

    void dma(DMA_transfer transfer) {
        unique_lock<mutex> l(m);
        // Ukoliko propusnica nije slobodna čeka
        while (stanje != SLOBODNA) {
            cv.wait(l);
        }
        // Kad se oslobodi
        stanje = DMA;
        // Čekanje za zauzimanje propusnice
        this_thread::sleep_for(milliseconds(700));
        // Ubacuje zahtev u red i obaveštava obrađivač
        transferi.push(transfer);
        cv_dma.notify_one();
    }

    DMA_transfer okidac_dma_kontrolera() {
        unique_lock<mutex> l(m);
        // Ukoliko nema zahteva čeka
        while (transferi.empty()) {
            cv_dma.wait(l);
        }
        // Kada se pojavi
        DMA_transfer t = transferi.front();
        transferi.pop();
        // Izvršavanje zahteva
        for (int i = 0; i < t.koliko; i++) {
            char ch = mem->citaj((t.odakle) + i);
            mem->pisi((t.kome) + i, ch);
        }
        // Oslobađa propusnicu i obaveštava druge procese
        stanje = SLOBODNA;
        cv.notify_one();
        return t;
    }
};

#endif // MAGISTRALA_H_INCLUDED

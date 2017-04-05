#ifndef BIBLIOTEKA_H_INCLUDED
#define BIBLIOTEKA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "primerak.h"

using namespace std;

class Biblioteka {
private:
    Primerak& primerak;
    mutex m;
    condition_variable cv;
    int primeraka;

public:
    Biblioteka(Primerak& pr, int br) : primerak(pr) {
        primeraka = br;
    }

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da iznajmi knjigu.
    // Metoda je blokirajuca - ukoliko nema slobodnih primeraka knjige, stajace u stanju cekanja dok se neki ne oslobodi.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.ceka kada nema slobodnih primeraka knjige.
    // Potrebno je pozvati metodu primerak.iznajmljen kada nema slobodnih primeraka knjige.
    void iznajmi(int rbr) {
        unique_lock<mutex> l(m);
        // Ako nema knjiga
        while (primeraka == 0) {
            // Čeka da se pojavi
            primerak.ceka(rbr);
            cv.wait(l);
        }
        // Kada se pojavi primerak
        primeraka--;
        primerak.iznajmljen(rbr);
    }

    // Metoda koju poziva nit koja simulira korisnika biblioteke kada on zeli da vrati knjigu koju je prethodno iznajmio.
    //
    // rbr - Redni broj clana
    //
    // Potrebno je pozvati metodu primerak.vracen kada je primerak vracen u biblioteku.
    void vrati(int rbr) {
        // Vraća primerak
        unique_lock<mutex> l(m);
        primeraka++;
        primerak.vracen(rbr);
        // I obaveštava drugu nit o tome
        cv.notify_one();
    }
};

#endif // BIBLIOTEKA_H_INCLUDED

#ifndef KOORDINATOR_H_INCLUDED
#define KOORDINATOR_H_INCLUDED

#include <mutex>
#include <condition_variable>

using namespace std;

class Koordinator {
private:
    mutex m;
    condition_variable cv;
    const int cekam_niti;     // Konstanta koja govori koliko niti se ceka na zavrsetak pre nego sto se aktivira poslednja.
    int gotovi;
public:
    Koordinator(int cn) : cekam_niti(cn) {
        gotovi = 0;
    }

    // Metoda koju na svom KRAJU zovu niti (a i b) koje ne cekaju (izvrsavaju se odmah).
    void zavrsio() {
        unique_lock<mutex> l(m);
        gotovi++;
        // Obaveštava drugu nit da se stanje promenilo
        cv.notify_one();
    }

    // Metodu koju na svom POCETKU zovu niti (c) koje cekaju (ne izvrsavaju se odmah).
    void cekam() {
        unique_lock<mutex> l(m);
        while (gotovi < cekam_niti) {
            // Nit čeka dok se uslov ne ispuni
            cv.wait(l);
        }
    }
};

#endif // KOORDINATOR_H_INCLUDED

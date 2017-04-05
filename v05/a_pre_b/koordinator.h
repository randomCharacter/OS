#ifndef KOORDINATOR_H_INCLUDED
#define KOORDINATOR_H_INCLUDED

#include <mutex>
#include <condition_variable>


using namespace std;

class Koordinator {
private:
    mutex m;
    condition_variable cv;
    bool prviGotov;
public:
    // Funkcija se poziva nakon sto je prva nit zavrsila svoj posao, kako bi signalizirala pokretanje druge niti.
    void prvi_zavrsio() {
        unique_lock<mutex> l(m);
        prviGotov = true;
        // Obaveštava drugu nit da se završio
        cv.notify_one();
    }

    // Blokirajuca funkcija koja se poziva na pocetku druge niti kako bi nit cekala dok se prva nit ne zavrsi.
    void drugi_ceka() {
        unique_lock<mutex> l(m);
        while (!prviGotov) {
            // Čeka dok se a ne izrši
            cv.wait(l);
        }
    }
};

#endif // KOORDINATOR_H_INCLUDED

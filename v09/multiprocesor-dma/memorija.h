#ifndef MEMORIJA_H_INCLUDED
#define MEMORIJA_H_INCLUDED

#include <mutex>

using namespace std;
using namespace chrono;

class Memorija {
private:
    mutex m;
    vector<char> mem;

public:
    Memorija(int bajtova) {
        mem.resize(bajtova, '?'); // ? - pošto se ne zna šta je u memoriji na početku
    }

    char citaj(int adresa) {
        unique_lock<mutex> l(m);
        // Vreme čekanja
        this_thread::sleep_for(milliseconds(300));
        // Vraća bajt sa date adrese
        return mem[adresa];
    }

    void pisi(int adresa, char vrednost) {
        unique_lock<mutex> l(m);
        // Vreme čekanja
        this_thread::sleep_for(milliseconds(300));
        // Piše bajt na datu adresu
        mem[adresa] = vrednost;
    }
};

#endif // MEMORIJA_H_INCLUDED

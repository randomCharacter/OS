#ifndef TENISKI_KLUB_H_INCLUDED
#define TENISKI_KLUB_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "podaci.h"
#include "mec.h"

#define MAX_TERENA 30

using namespace std;

class Teniski_klub {                             //Klasa deljenog resursa. Pravi se maksimalno 30 terena, ali ce u realnosti biti samo 3 terena.
private:
    Mec& mec;
    mutex m;
    condition_variable *cv;
    bool *slobodan;
public:
    Teniski_klub(Mec& m, int ukupno_terena) : mec(m) {
        cv = new condition_variable[ukupno_terena];
        slobodan = new bool[ukupno_terena];
        for (int i = 0; i < ukupno_terena; i++) {
            slobodan[i] = true;
        }
    }

    // Metoda koju poziva nit koja simulira mec kako bi izvrsila teniski mec.
    // Metoda je blokirajuca - ako je zeljeni teren zauzet, izvrsenje ce blokirati dok se on ne oslobodi.
    //
    // broj_meca - Redni broj meca
    // na_terenu - Indeks terena na kojem mec treba da se odigra
    //
    // Potrebno je pozvati metodu mec.ceka kada je izabrani teren zauzet i mec mora da ceka.
    // Potrebno je pozvati metodu mec.pocinje kada se isprazni izabrani teren i mec moze da pocne.
    // Potrebno je pozvati metodu mec.zavrsen kada se mec zavrsi i mogu se proslediti izmereni vremenski intervali.
    void odigraj_mec(int broj_meca, int na_terenu) {
        Podaci p;
        p.dosao = system_clock::now();
        {
            unique_lock<mutex> l(m);
            // Ako je teren zauzet
            while (!slobodan[na_terenu]) {
                mec.ceka(broj_meca, na_terenu);
                cv[na_terenu].wait(l);
            }
            // Kad se teren oslobodi
            p.pocetak = system_clock::now();
            slobodan[na_terenu] = false;
            mec.pocinje(broj_meca, na_terenu);
        }
        this_thread::sleep_for(seconds(rand() % 5 + 1));

        unique_lock<mutex> l(m);
        // Kad se meč završi
        p.trajanje = system_clock::now() - p.pocetak;
        slobodan[na_terenu] = true;
        mec.zavrsen(broj_meca, p);
        cv[na_terenu].notify_one();
    }
};

#endif // TENISKI_KLUB_H_INCLUDED

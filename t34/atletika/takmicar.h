#ifndef TAKMICAR_H_INCLUDED
#define TAKMICAR_H_INCLUDED

#include <iostream>

#include "povratna.h"

using namespace std;

class Takmicar {
public:
    // rbr - Redni broj takmicara
    void skakac_ceka(int rbr) {
        cout << "Takmicar sa brojem " << rbr << " (skakac) ceka da stupi na zaletiste." << endl;
    }

    // rbr - Redni broj takmicara
    void bacac_ceka(int rbr) {
        cout << "Takmicar sa brojem " << rbr << " (bacac) ceka da stupi na zaletiste." << endl;
    }

    // rbr - Redni broj takmicara
    void skakac_skace(int rbr) {
        cout << "Takmicar sa brojem " << rbr << " (skakac) stupio je na zaletiste." << endl;
    }

    // rbr - Redni broj takmicara
    void bacac_baca(int rbr) {
        cout << "Takmicar sa brojem " << rbr << " (bacac) stupio je na zaletiste." << endl;
    }

    // rbr - Redni broj takmicara
    // rez - Rezultati takmicarskog nastupa
    void skakac_zavrsio(int rbr, Povratna_vrednost rez) {
    	cout << "Takmicar sa brojem " << rbr << " skocio " << rez.rezultat << " metara"
             << ", a cekao " << rez.trajanje.count() << " milisekundi. " << endl;
    }

    // rbr - Redni broj takmicara
    // rez - Rezultati takmicarskog nastupa
    void bacac_zavrsio(int rbr, Povratna_vrednost rez) {
        cout << "Takmicar sa brojem " << rbr << " bacio koplje " << rez.rezultat << " metara"
             << ", a cekao " << rez.trajanje.count() << " milisekundi. " << endl;
    }
};

#endif // TAKMICAR_H_INCLUDED

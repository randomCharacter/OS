#ifndef KUPAC_H_INCLUDED
#define KUPAC_H_INCLUDED

#include <iostream>

using namespace std;

class Kupac {
public:
    // rbr - Redni broj kupca
    void ceka(int rbr) {
        cout << "Kupac " << rbr << " ceka jer su kabine za probu zauzete." << endl;
    }

    // rbr - Redni broj kupca
    void proba_odecu(int rbr) {
        cout << "Kupac " << rbr << " ulazi u kabinu za probu odece." << endl;
    }

    // rbr - Redni broj kupca
    // pv  - Rezultat kupovine
    void zavrsio(int rbr, povratna_vrednost pv) {
        if (pv.kupio)
            cout << "Kupac " << rbr << " je kupio odecu. Cekao na kabinu: "
                 << pv.cekao_na_kabinu.count() << " milisekundi." << endl;
        else {
            cout << "Kupac " << rbr << " nije kupio odecu. Cekao na kabinu: "
                 << pv.cekao_na_kabinu.count() << " milisekundi." << endl;
            this_thread::sleep_for(seconds(1)); //kupac trazi novi komad odece
        }
    }
};

#endif // KUPAC_H_INCLUDED

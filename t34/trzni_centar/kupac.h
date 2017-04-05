#ifndef KUPAC_H_INCLUDED
#define KUPAC_H_INCLUDED

#include <iostream>

using namespace std;

class Kupac {
public:
    // rbr       - Redni broj kupca
    // kasa      - Kasa (redni broj) na kojoj kupac ceka da obavi kupovinu
    void ceka(int rbr, int kasa) {
        cout << "Kupac " << rbr << " mora da ceka da se kasa " << kasa << " oslobodi." << endl;
    }

    // rbr       - Redni broj kupca
    // kasa      - Kasa (redni broj) na kojoj kupac obavlja kupovinu
    // kolicina  - Broj artikala koje kupac zeli da kupi
    void kupuje(int rbr, int kasa, int kolicina) {
        cout << "Kupac " << rbr << " dolazi na kasu " << kasa << " da bi kupio " << kolicina << " artikala." << endl;
    }

    // rbr       - Redni broj kupca
    void zavrsio(int rbr) {
        cout << "Kupac " << rbr << " je zavrsio." << endl;
    }
};

#endif // KUPAC_H_INCLUDED

#ifndef PRIMERAK_H_INCLUDED
#define PRIMERAK_H_INCLUDED

#include <iostream>

using namespace std;

class Primerak {
public:
    // rbr - Redni broj clana
    void ceka(int rbr) {
        cout << "Clan sa rednim brojem " << rbr << " ceka na slobodan primerak knjige." << endl;
    }

    // rbr - Redni broj clana
    void iznajmljen(int rbr) {
        cout << "Clan sa rednim brojem " << rbr << " iznajmio je primerak knjige." << endl;
    }

    // rbr - Redni broj clana
    void vracen(int rbr) {
        cout << "Clan sa rednim brojem " << rbr << " vratio je primerak knjige." << endl;
    }
};

#endif // PRIMERAK_H_INCLUDED

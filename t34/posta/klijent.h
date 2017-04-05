#ifndef KLIJENT_H_INCLUDED
#define KLIJENT_H_INCLUDED

#include <iostream>

using namespace std;

class Klijent {
public:
    // rbr   - Redni broj klijenta
    // svota - Svota koju klijent zeli da uplati
    void ceka(int rbr, int svota) {
        cout << "Klijent broj: " << rbr << " ceka na salteru, zeli da uplati " << svota << " hiljada dinara." << endl;
    }

    // rbr    - Redni broj klijenta
    // salter - Salter na kojem ce klijent obaviti uplatu
    // svota  - Svota koju klijent zeli da uplati
    void uplacuje(int rbr, int salter, int svota) {
        cout << "Klijent broj: " << rbr << " (" << svota << ") salter " << salter << endl;
    }

    // rbr             - Redni broj klijenta
    // salter          - Salter na kojem ce klijent obaviti uplatu
    // ukupno_uplaceno - Ukupan saldo na salteru (od svih dotadasnjih uplata)
    void napusta(int rbr, int salter, int ukupno_uplaceno) {
        cout << "Klijent broj: " << rbr << " napusta salter " << salter << " (ukupno uplaceno na salteru: " << ukupno_uplaceno << ")" << endl;
    }
};

#endif // KLIJENT_H_INCLUDED

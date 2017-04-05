#ifndef KREDIT_H_INCLUDED
#define KREDIT_H_INCLUDED

#include <iostream>

#include "valuta.h"

using namespace std;

class Kredit {
public:
    // rbr - Redni broj klijenta
    // svota - Novcani iznos koji je potreban klijentu
    // valuta - Valuta u kojem je novcani iznos
    void ceka(int rbr, int svota, Valuta valuta) {
       cout << "Klijent: " << rbr << " mora da ceka kako bi dobio kredit od " << svota << " " << naziv_valute(valuta) << endl;
    }

    // rbr - Redni broj klijenta
    // svota - Novcani iznos koji je potreban klijentu
    // saldo - Ukupna raspoloziva sredstva u banci za trazenu valutu (nakon odobrenja kredita)
    // valuta - Valuta u kojem je novcani iznos
    void dobio(int rbr, int svota, int saldo, Valuta valuta) {
        cout << "Klijent: " << rbr << " dobio " << svota << ", u banci ostalo: " << saldo << ", valuta: " << naziv_valute(valuta) << endl;
    }

    // rbr - Redni broj klijenta
    // svota - Novcani iznos koji je potreban klijentu
    // saldo - Ukupna raspoloziva sredstva u banci za trazenu valutu (nakon sto je iznos kredita vracen)
    // valuta - Valuta u kojem je novcani iznos
    void vratio(int rbr, int svota, int saldo, Valuta valuta) {
        cout << "Klijent: " << rbr << " vratio " << svota << ", u banci ostalo: " << saldo << ", valuta: " << naziv_valute(valuta) << endl;
}
};

#endif // KREDIT_H_INCLUDED

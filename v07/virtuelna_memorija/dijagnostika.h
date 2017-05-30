#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>

#include "naredba.h"

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;

    // n - naredba koju je potrebno ispisati u string
    //
    // Vraća string koji sadrži opis naredbe
    string str_naredba(Naredba n) {
        string rezultat = n.tip;
        if (n.tip == "malloc") {
        	rezultat += "(" + to_string(n.kolicina_memorije) + ")";
        } else if (n.tip == "repeat") {
        	rezultat += "[" + to_string(n.ponavljanja) + "]" + "malloc" + "(" + to_string(n.kolicina_memorije) + ")";
        }
        return rezultat;
    }

public:
    // n - naredba koja je pokrenuta
    void pokrenuta_naredba(Naredba n) {
        unique_lock<mutex> l(term_mx);
        cout << "[NAREDBA] Pokrece se izvrsenje: " << str_naredba(n) << endl;
    }

    // n            - naredba koja je izvršena
    // slobodno_ram - koliko RAM memorije je ostalo slobodno nakon izvršenja naredbe
    // zauzeto_vm   - koliko VM je zauzeto do sada (ukupno, ne samo ovom naredbom)
    void izvrsena_naredba(Naredba n, int slobodno_ram, int zauzeto_vm) {
        unique_lock<mutex> l(term_mx);
        cout << "[NAREDBA] Izvrsena je: " << str_naredba(n) << ", preostalo RAM memorije: " << slobodno_ram << ", zauzeto virtuelne memorije: " << zauzeto_vm << endl;
    }

    // upotrebljeno - koliko VM je ukupno zauzeto do sada
    void izvrseno_zauzimanje_vm(int upotrebljeno) {
        cout << "[OS] Izvrseno zauzimanje virtuelne memorije. Trenutno se koristi: " << upotrebljeno << " MB" << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

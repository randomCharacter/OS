/*
Kreirati globalnu celobrojnu promeljivu brojač. Nakon toga kreirati 2 funkcije inkrement i dekrement koje povećavaju i smanjuju dati brojač ITERACIJA puta, pri čemu je ITERACIJA konstanta koja predstavlja proizvoljno velik broj (npr. 100000000). Kreirati jednu nit od funkcije inkrement i jednu nit od funkcije dekrement.

Nakon završatka rada niti ispisati vrednosti brojača. Da li je data vrednost očekivana?
*/

#include <iostream>
#include <thread>

using namespace std;

const int ITERACIJA = 100000000;

int brojac = 0;

// Funkcija koja inkrementira brojač ITERACIJA puta
void increment() {
    for (int i = 0; i < ITERACIJA; i++) {
        brojac++;
    }
}

// Funkcija koja dekrementira brojač ITERACIJA puta
void decrement() {
    for (int i = 0; i < ITERACIJA; i++) {
        brojac--;
    }
}

int main() {
    // Pozivanje funkcija iz različitih niti
    thread tInc(increment);
    thread tDec(decrement);

    // Čekanje da se niti završe
    tInc.join();
    tDec.join();

    // Ispis rezultata
    cout << brojac << endl;

    // Dobijeni rezultat nije nula zbog štetnog preplitanja koje se desilo između
    return 0;
}

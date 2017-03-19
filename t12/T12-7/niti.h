#ifndef NITI_H
#define NITI_H

#include <thread>
#include <vector>

using namespace std;

// Implementacija operacije XOR nad ulaznim vektorima A i B izvršene uz pomoć dve niti (svaka nit obrađuje polovinu)
//
// a - Vektor A (16 binarnih cifara)
// b - Vektor B (16 binarnih cifara)
//
// Rezultat izvršavanja treba da bude vector<int> koji sadrži rezultat izvršenja XOR operacije nad ulaznim vektorima.
//
vector<int> izracunaj(vector<int>& a, vector<int>& b) {
    thread t[2];

    int n = a.size();

    vector<int> c(a.size());

    for (int i = 0; i < 2; i++) {
        t[i] = thread(izvrsiXOR, a.begin() + n * i / 2, a.begin() + n * (i+1) / 2, b.begin() + n * i / 2, c.begin() + n * i / 2);
    }

    for (int i = 0; i < 2; i++) {
        t[i].join();
    }

    return c;
}

#endif // NITI_H

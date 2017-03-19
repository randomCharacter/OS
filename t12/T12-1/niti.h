#ifndef NITI_H
#define NITI_H

#include <thread>
#include <vector>

using namespace std;

// Implementacija sabiranja korespondentnih elemenata celih vektora A i B pokretanjem onoliko niti koliko ima elemenata u vektorima
//
// a - Vektor A
// b - Vektor B
//
// Rezultat izvršavanja treba da bude vector<int> koji sadrži rezultat sabiranja korespondentnih elemenata.
//
vector<int> izracunaj(vector<int>& a, vector<int>& b) {
    int n = a.size();

    thread t[n];
    vector<int> s(a.size());
    cit it_a = a.begin();
    cit it_b = b.begin();
    vector<int>::iterator rez = s.begin();

    for (int i = 0; i < n; i++) {
        t[i] = thread(saberiPar, it_a, it_b, rez);
        it_a++;
        it_b++;
        rez++;
    }

    for (int i = 0; i < n; i++) {
        t[i].join();
    }

    return s;
}

#endif // NITI_H

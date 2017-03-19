#include <iostream>
#include <vector>
#include <thread>
#include "vise_niti.h"
#include "suma_vektora.h"

using namespace std;

// v - vektor cije elemente treba sumirati
// n - koliko niti treba pokrenuti
// povratna vrednost - suma svih elemenata vektora, izracunata pokretanjem n niti (svaka nit treba da obradi jedan deo elemenata)
double sumiraj(vector<double> v, int n) {
    // Kreiranje rezultata
    double zbir[n];
    // Niz iteratora
    ci it[n+1];
    // Niz niti
    thread t[n];
    // rezultat
    double s;

    // Postavljanje iteratora
    for (int i = 0; i <= n; i++) {
        it[i] = v.begin() + v.size() * i / n;
    }

    // Kreiranje niti
    for (int i = 0; i < n; i++) {
        t[i] = thread(f, v.begin() + v.size() * i / n, v.begin() + v.size() * (i+1) / n, ref(zbir[i]));
    }

    // Čekanje kraja niti i računanje zbira
    for (int i = 0; i < n; i++) {
        t[i].join();
        s += zbir[i];
    }

    return s;
}

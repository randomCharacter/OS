#include <iostream>
#include <vector>
#include <thread>
#include "vise_niti.h"
#include "suma_vektora.h"

using namespace std;

// v - vektor cije elemente treba sumirati
// povratna vrednost - suma svih elemenata vektora, izracunata pokretanjem 2 niti (svaka treba da obradi jednu polovinu elemenata)
double sumiraj(vector<double> v) {
    // Promenljive za čuvanje rezultata
    double zbir1, zbir2;
    // Iteratori
    ci pocetak = v.begin();
    ci sredina = v.begin() + v.size() / 2;
    ci kraj = v.end();

    // Pozivanje niti
    thread t1(f, pocetak, sredina, ref(zbir1));
    thread t2(f, sredina, kraj, ref(zbir2));

    // Čekanje da se niti završe
    t1.join();
    t2.join();

    return zbir1 + zbir2;
}

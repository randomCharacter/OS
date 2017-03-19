#ifndef NITI_H
#define NITI_H

#include <thread>
#include <list>

using namespace std;

// Implementacija obrtanja redosleda elemenata u listi pokretanjem jedne nove niti
//
// ulazna  - Ulazna lista, koja se obrađuje
//
// Rezultat je rezultujuća lista (popunjena elementima u obrnutom redosledu)
//
list<int> izracunaj(list<int>& ulazna) {
    list<int> a;
    thread t(obrniRedosled, ulazna, ref(a));
    t.join();
    return a;
}

#endif // NITI_H

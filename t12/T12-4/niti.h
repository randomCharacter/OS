#ifndef NITI_H
#define NITI_H

#include <thread>
#include <vector>

using namespace std;

// Implementacija pokretanja tri niti koje treba da pretraže (svaka svoju trećinu) ulazni vektor brojeva i nađu element najbliži nuli
//
// ulaz - Ulazni vektor brojeva koji se pretražuje
//
// Povratna vrednost je nađeni element, najbliži nuli od svih elemenata koji su obuhvaćeni pretragom
//
double izracunaj(vector<double> ulaz) {
    thread t[3];

    double najblizi[3];

    int n = ulaz.size();

    for (int i = 0; i < 3; i++) {
        t[i] = thread(pretrazi, ulaz.begin() + n * i / 3, ulaz.begin() + n * (i+1) / 3, ref(najblizi[i]));
    }

    for (int i = 0; i < 3; i++) {
        t[i].join();
    }

    double m = abs(najblizi[0]) < abs(najblizi[1]) ? najblizi[0] : najblizi[1];

    m = abs(m) < abs(najblizi[2]) ? m : najblizi[2];
}


#endif // NITI_H

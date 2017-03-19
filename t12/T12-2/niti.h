#ifndef NITI_H
#define NITI_H

#include <thread>
#include <list>

using namespace std;

// Implementacija pokretanja više niti koje treba da ispišu svoje redne brojeve (koristeći funkciju "ispisi")
//
// brojNiti - Broj niti koje je potrebno startovati
//
void izracunaj(int brojNiti) {
   thread niti[brojNiti];   // Kreiranje niza od n niti
   for (int i = 0; i < brojNiti; i++) {
        niti[i] = thread(ispisi, i+1);  // Kreiranje niti od funkcije "ispisi". Svaka nit dobija svoj redni broj
   }
   for (int i = 0; i < brojNiti; i++) {
        niti[i].join(); // Čekanje da terminišu sve kreirane niti
   }
}

#endif // NITI_H

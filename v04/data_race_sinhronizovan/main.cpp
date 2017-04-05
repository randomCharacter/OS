/*
Izmeniti program data_race.cpp i realizovati zaštitu pristupa brojaču uz pomoć klase mutex i njenih operacija lock() i unlock(). Primetiti koliko sada traje izvršavanje programa.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

const int ITERACIJA = 100000000;

int brojac = 0;

// mutex za zaštitu brojača
mutex m;

// Funkcija koja inkrementira brojač ITERACIJA puta
void increment() {
    for (int i = 0; i < ITERACIJA; i++) {
        m.lock();
        brojac++;
        m.unlock();
    }
}

// Funkcija koja dekrementira brojač ITERACIJA puta
void decrement() {
    for (int i = 0; i < ITERACIJA; i++) {
        m.lock();
        brojac--;
        m.unlock();
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

    // Dobijeni rezultat jeste nula, ali se vreme izvšavanja drastično povećalo
    return 0;
}

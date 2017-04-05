// Definisati klasu Parking koja modeluje Parking prostor kapaciteta N mesta.
// Kapacitet Parkinga proslediti kao argument konstruktoru, pri instanciranju deljene promenljive.
//
// Ova klasa ima operacije:
//    void Parking::udji();
//    void Parking::izadji();
//
// Automobili koji dolaze na Parking su predstavljeni nitima.
// Za ulazak na Parking, automobil poziva metodu udji().
// Za izlazak sa Parkinga, automobil poziva metodu izadji().
// Automobil se na Parkingu zadrzava od 1 do 3 sekunde.
// Pri ulasku, ukoliko je Parking zauzet, automobil mora da saceka da se neko parking mesto oslobodi.
//
// Implementirati oznacene metode u klasi Parking u fajlu parking.h

#include <thread>

#include "parking.h"

using namespace std;

const int KAPACITET = 3;

void automobil(Parking& p, int rba) {
    p.udji(rba);
    this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));
    p.izadji(rba);
}

const int automobila = 10;

int main() {
    Automobil a;
    Parking p(a, KAPACITET);
    thread t[automobila];
    for(int i = 0; i < automobila; i++)
        t[i] = thread(automobil, ref(p), i + 1);
    for(int i = 0; i < automobila; i++)
        t[i].join();
}

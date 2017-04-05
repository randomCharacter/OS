// Modelovati placanje robe u trznom centru. U trznom centru postoje 2 kase za placanje.
// Kupac pri placanju staje u red na onu kasu na kojoj ceka manji broj kupaca.
// Kupac vrsi placanje pozivom metode kupi() koja kao parametar dobija broj artikala koje kupac placa.
// Placanje robe traje onoliko sekundi koliko ima artikala.
// Povratna vrednost metode je identifikator kase na kojoj je placanje izvrseno.
//
// Implementirati oznacene metode klase Trzni_centar u fajlu trzni_centar.h

#include <thread>

#include "trzni_centar.h"

using namespace std;
using namespace chrono;

void kupac(Trzni_centar &p, int kolicina, int rbk) {
    p.kupi(rbk, kolicina); //kupac ide u prodavnicu i vrsi kupovinu prosledjene kolicine robe (posle moguceg cekanja zbog guzve na kasi)
}

const int KUPACA = 10;

void testirajSve() {
    Kupac k;
    Trzni_centar p(k);
    thread t[KUPACA];

    for (int i = 0; i < KUPACA; i++)
        t[i] = thread(kupac, ref(p), i + 1, i + 1);

    for (int i = 0; i < KUPACA; i++)
        t[i].join();
}

int main() {
    testirajSve();

    return 0;
}


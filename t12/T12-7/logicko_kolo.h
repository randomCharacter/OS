#ifndef LOGICKO_KOLO_H
#define LOGICKO_KOLO_H

#include <vector>

using namespace std;

typedef vector<int>::const_iterator cit;

// Implementacija XOR operacije - treba da se koristi kao funkcija niti
//
// a_pocetak - Iterator na prvi element vektora A koji se obrađuje
// a_kraj    - Iterator iza poslednjeg elementa vektora A koji se obrađuje
// b_pocetak - Iterator na prvi element vektora B koji se obrađuje
// rez       - Iterator u rezultujućem vektoru - elementi počevši sa ovom pozicijom trebaju biti popunjeni rezultatima XOR operacije
//
void izvrsiXOR(cit a_pocetak, cit a_kraj, cit b_pocetak, vector<int>::iterator rez) {
    for (cit it_a = a_pocetak, it_b = b_pocetak; it_a != a_kraj; it_a++, it_b++, rez++) {
        *rez = *it_a ^ *it_b;
    }
}


#endif // LOGICKO_KOLO_H

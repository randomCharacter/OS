/*
Napraviti konkurentni program koji izračunava sumu svih elemenata vektora, koristeći funkciju f():

typedef vector<double>::const_iterator ci;
void f(ci pocetak, ci kraj, double& zbir);

Funkcija f nalazi se u fajlu suma_vektora.cpp.

Potrebno je implementirati i funkciju sumiraj (fajl vise_niti.cpp) koja treba da pokrene dve niti
i da svakoj niti prosledi polovinu elemenata ulaznog vektora. Obe niti treba da koriste funkciju f
kako bi izračunale sumu svog dela vektora.

Napomena: ovakva optimizacija sumiranja je značajna kada se radi na dvojezgarnom procesoru za
vektore velike dužine.
*/

#include <iostream>
#include "suma_vektora.h"
#include "vise_niti.h"

using namespace std;

void testirajSumu() {
    vector<double> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double zbir;
    f(v.begin(), v.end(), zbir);

    cout << "Zbir vektora: " << zbir << endl;
}

void testirajNiti() {
    vector<double> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double zbir;
    zbir = sumiraj(v);

    cout << "Zbir vektora, koriscenjem 2 niti: " << zbir << endl;
}

int main() {
    testirajSumu();
    testirajNiti();

    return 0;
}

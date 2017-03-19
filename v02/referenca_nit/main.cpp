/*
Definisati funkciju: void increment(int& a);
koja povećava (za jedan) vrednost argumenta.

Napraviti program koji:
a) poziva funkciju increment()
b) stvara nit od funkcije increment()
*/

#include <iostream>
#include <thread>

using namespace std;

// Funkcija koja inkrementira prosleđeni parametar
void increment(int &a) {
    a++;
}

int main() {
    int a = 0;
    int b = 0;

    // Obično pozivanje funkcije
    increment(a);

    // Pozivanje funkcije iz niti
    // ref() - operator reference, obavezan ako rezultat treba da se sačuva
    thread t(increment, ref(b));

    // Tek nakon izvršenja ove linije može se smatrati da je b dobilo željenu vrednost
    t.join();

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    return 0;
}

/*
Napraviti konkurentni program koji pokrece vise niti u petlji.
Svakoj niti proslediti njen redni broj prilikom kreiranja i
svaka nit treba da ispise dati broj u okviru tela niti.
*/

#include <iostream>
#include <thread>

using namespace std;

void f(int rbr) {
    cout << rbr << endl;
}

const int BR_NITI = 10;

int main() {
    // Kreiranje niza niti
    thread t[BR_NITI];

    // Inicijalizacija svh niti
    for (int i = 0; i < BR_NITI; i++) {
        t[i] = thread(f, i);
    }

    // Čekanje da se sve niti izvrše
    for (int i = 0; i < BR_NITI; i++) {
        t[i].join();
    }
    return 0;
}

/*
Napraviti konkurentni program koji modeluje klasu brojača. Interfejs klase sadrži sledeće metode:

class Brojac {
   public:
      void inc();
      void dec();
      friend ostream& operator<<(ostream& , Brojac& );
};

Metode inc i dec povećavaću i smanjuju vrednost brojača respektivno. Operator << služi za ispis brojača na ekran.

Klasa treba da zaštiti konzistentnost brojača u datim metodama uz pomoć klasa mutex i unique_lock.

Kreirati 1 globalni objekat brojača kome će pristupati 2 niti.

Kreirati 2 niti pri čemu jedna nit poziva metodu uvećavanja brojača 1000000 puta a druga metodu smanjivanja brojača 1000000 puta. Na kraju programa ispisati konačnu vrednost brojača nakon uvećavanja i smanjivanja.
*/

#include <thread>

#include "brojac.h"

const int ITERACIJA = 100000000;

Brojac brojac;

// Funkcija koja inkrementira brojač ITERACIJA puta
void increment() {
    for (int i = 0; i < ITERACIJA; i++) {
        brojac.inc();
    }
}

// Funkcija koja dekrementira brojač ITERACIJA puta
void decrement() {
    for (int i = 0; i < ITERACIJA; i++) {
        brojac.dec();
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

    return 0;
}

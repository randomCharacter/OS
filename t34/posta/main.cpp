// Modelovati salter salu u posti u kojoj postoje 2 saltera.

// Vremensko trajanje uplate (boravak klijenta na salteru) je srazmerno
// velicini uplate. Za svaku uplacenu hiljadu dinara klijent ceka 1 sec.
// Na salteru se moze uplatiti maksimalno 4 hiljada dinara.
// (podrazumeva se da je ispravna vrednost prosledjena klijentu
// pri stvaranju niti).

// Kada klijent zeli da uplati sredstava, on poziva operaciju uplati(),
// cime prakticno ulazi u postu i staje u red.
// Povratna vrednost ove operacije je broj saltera na kojem je klijent
// izvrsio uplatu i svota koja je do tog trenutka na salteru uplacena.
//
// Implementirati oznacene metode klase Posta u fajlu posta.h

#include <thread>

#include "povratna.h"
#include "posta.h"
#include "klijent.h"

using namespace std;
using namespace chrono;


void klijent(Posta& p, int svota, int rbk) {
    p.uplati(rbk, svota);
}

const int KLIJENATA = 7;

int testirajSve() {
    Klijent k;
    Posta p(k);
    thread t[KLIJENATA];

    for(int i = 0; i < KLIJENATA; i++)
        t[i]=thread(klijent, ref(p), i * 3 % 4 + 1, i + 1); //za svotu se prosledjuje uvek razlicita vrednost, zavisno od rednog broja klijenta

    for(int i = 0; i < KLIJENATA; i++)
        t[i].join();
}

int main() {
    testirajSve();

    return 0;
}

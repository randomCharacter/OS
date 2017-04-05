// Modelovati soping na rasprodaji.
// Kupci dolaze u prodavnicu da kupe odecu.
//
// Kupac najpre probava odecu u jednoj od kabina za probavanje.
// Broj kabina za probavanje prosledjuje se pri instanciranju klase Prodavnica.
// Ako su sve kabine zauzete, kupac mora da saceka sa probavanjem dok se neka kabina ne oslobodi.
//
// Nakon probavanja odece, kupac vrsi kupovinu, ako mu odeca odgovara.
// Sansa da mu odeca odgovara je 50%.
// Probavanje odece traje 1 sekundu.
//
// Metoda "kupi" vraca informaciju da li je kupac kupio odecu i koliko dugo je cekao da udje u kabinu.
//
// Data funkcija "kupac" modeluje ponasanje kupca. Ako mu isprobana odeca ne odgovara, kupac odlazi da pronadje
// drugi komad odece i onda ponovo odlazi da proba odecu u kabini.
//
// Implementirati oznacene metode klase Prodavnica u fajlu prodavnica.h

#include <thread>

#include "prodavnica.h"

using namespace std;
using namespace chrono;

void kupac(Prodavnica &p, int id_kupca) {
    povratna_vrednost pv;
    do {
        pv = p.kupi(id_kupca);
        if (!pv.kupio)
            this_thread::sleep_for(seconds(1)); // Ako nije kupio, kupac trazi novi komad odece
    } while (!pv.kupio);            // Radi probu sve dok ne uspe da pronadje odgovarajucu odecu.
}

const int KUPACA = 10;

int testirajSve() {
    Kupac k;
    Prodavnica p(k, 3);
    thread kupci[KUPACA];
    for (int i = 0; i < KUPACA; ++i)
        kupci[i] = thread(kupac, ref(p), i+1);

    for (int i = 0; i < KUPACA; ++i)
        kupci[i].join();
}

int main() {
    testirajSve();

    return 0;
}

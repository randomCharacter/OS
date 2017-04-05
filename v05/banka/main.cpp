// Napraviti konkurentni program koji modeluje kreditno poslovanje banke.
// Banka odobrava kredite u dinarima i u evrima.
//
// Klijent trazi kredit pozivanjem operacije uzmi_kredit(),
// kojoj prosledjuje svotu koju zeli da pozajmi od banke i valutu u kojoj zeli da pozajmi.
// Klijent neko vreme koristi pozajmljena sredstva, pa ih vrati banci
// pozivanjem operacije vrati_kredit().
//
// Banka inicijalno poseduje odredjene svote dinara i evra
// na dva razlicita racuna, koje pozajmljuje.
// Banka odobrava kredite dok ima sredstava.
// Kada vise nema sredstava, banka ceka da klijenti vrate
// pretodno odobrene kredite pre nego sto odobri sledeci kredit.
// Banka odobrava kredite u proizvoljnom redosledu.
//
// Banka tezi tome da klijent ciji je zahtev moguce ispuniti (postoje sredstva) ne ceka na kredit.
//
// Implementirati oznacene metode u klasi Banka u fajlu banka.h

#include <thread>
#include <vector>

#include "valuta.h"
#include "banka.h"
#include "kredit.h"

using namespace std;

void klijent(Banka& b, int svota, Valuta valuta, int rbk) {
   b.uzmi_kredit(rbk, svota, valuta);
   // klijent koristi pozajmljeni novac
   this_thread::sleep_for(chrono::seconds(1));
   b.vrati_kredit(rbk, svota, valuta);
}

const int DSVOTA = 30;
const int ESVOTA = 20;
const int DIN_KLIJENATA = 9;
const int EUR_KLIJENATA = 9;

void testirajSve() {
    Kredit k;
    Banka b{k, DSVOTA, ESVOTA};
    thread t[DIN_KLIJENATA];
    thread t1[EUR_KLIJENATA];

    vector<int> d_kolicine = {20, 5, 7, 13, 14, 4, 30, 6, 9};
    vector<int> e_kolicine = {10, 10, 5, 20, 6, 7, 15, 3, 12};

    for(int i = 0; i < DIN_KLIJENATA; i++)
        t[i] = thread(klijent, ref(b), d_kolicine[i], Valuta::DINAR, i);
    for(int i = 0; i < EUR_KLIJENATA; i++)
        t1[i] = thread(klijent, ref(b), e_kolicine[i], Valuta::EURO, DIN_KLIJENATA + i);

    for(int i = 0; i < DIN_KLIJENATA; i++)
        t[i].join();
    for(int i = 0; i < EUR_KLIJENATA; i++)
        t1[i].join();
}

int main() {
    testirajSve();

    return 0;
}

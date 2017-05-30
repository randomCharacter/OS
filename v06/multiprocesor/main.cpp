// Napisati konkurentni program koji modeluje pristupanje više procesa deljenoj memoriji.
//
// Deljena memorija je predstavljena klasom Memorija. Prilikom instanciranja se zadaje veličina memorije (ukupan broj lokacija koje postoje u memoriji). Upis vrednosti u memoriju se obavlja pozivom operacije Memorija.pisi, a čitanje pozivom operacije Memorija.citaj. I čitanje i upis svake vrednosti traje po 1 sekundu. Ove dve metode kao parametar prihvataju memorijsku lokaciju (ona je izražena u bajtima/karakterima). Interno, sve lokacije koji čine memoriju mogu se predstaviti karakterima (tip char).
//
// Iako metode Memorija.pisi i Memorija.citaj memoriju adresiraju po lokacijama, interno pristup pojedinačnoj lokaciji u klasi Memorija pogađa celu memorijsku reč kojoj lokacija pripada. Veličina memorijske reči je 4 bajta, tako da npr ako je zahtevan pristup lokacijama 0 i 3, to je pristup prvoj memorijskoj reči kojoj obe ove lokacije pripadaju.
//
// Tokom upisa u memorijsku lokaciju ekskluzivno se zaključava cela memorijska reč kojoj ta lokacija (bajt) pripada. Dakle, operacija upisa ne dozvoljava da paralelno sa njom teče bilo koja druga operacija (čitanje ili neki drugi upis u istu memorijsku reč).
//
// Tokom čitanja memorijske lokacije reč kojoj ta lokacija (bajt) pripada se može istovremeno čitati iz drugih niti.
//
// Treba omogućiti istovremeno pristupanje memorijskim lokacijama koje ne pripadaju istoj reči.
//
// Ako istovremeno postoje i procesi koji žele da čitaju i procesi koji žele da pišu u istu memorijsku reč, prvo treba procesi koji čitaju da dobiju pristup.
//
// Implementirati označene metode, konstruktor i dodati privatne članove u klasu Memorija (fajl memorija.h).

#include <vector>
#include <thread>

#include "memorija.h"

using namespace std;
using namespace chrono;

// Struktura koja će se koristiti radi formiranja test scenarija:
struct Pristup_memoriji {
    Stanje tip_pristupa;                // Tip pristupa memoriji
    int adresa;                         // Adresa kojoj se pristupa
    char vrednost;                      // Vrednost koja se upisuje (samo ako je tip pristupa upis u memoriju)
    Pristup_memoriji(Stanje tip, int adr, char v = 0) : tip_pristupa(tip), adresa(adr), vrednost(v) {}
};

// Funkcija koju koriste niti koje simuliraju procese koji pristupaju memoriji:
void proces(double pauza, Memorija& mem, const vector<Pristup_memoriji>& pristupi, int rbp) {
    this_thread::sleep_for(duration<double>(pauza));
    for(auto it = pristupi.begin(); it != pristupi.end(); it++) {
        if(it->tip_pristupa == CITANJE)
            mem.citaj(rbp, it->adresa);
        else
            mem.pisi(rbp, it->adresa, it->vrednost);
    }
}

class Test {
private:
    Dijagnostika d;
    Memorija& memorija;
    vector<thread*> niti;
    int rbp;
public:
    Test(Memorija& m) : memorija(m), rbp(1) {}

    void nit(double pauza, const vector<Pristup_memoriji>& pristupi) {
        niti.push_back(new thread(proces, pauza, ref(memorija), pristupi, rbp++));
    }

    void pokreni() {
        for (vector<thread*>::iterator it = niti.begin(); it != niti.end(); it++) {
            (*it)->join();
            delete *it;
        }

        niti.clear();
        cout << endl;
    }
};

void testirajIstaMemorijskaRec() {
    cout << "Test operacija nad istom memorijskom reci" << endl;
    cout << "*****************************************" << endl;

    Dijagnostika d;
    Memorija mem(d, 4);
    Test t(mem);

    // Četiri niti upisuju vrednosti A...D u četiri adrese prve memorijske reči.
    // Prema uslovima zadatka, ovi upisi treba da se obave jedan za drugim (iako su niti pokrenute istovremeno):
    t.nit(0.0, {{Pristup_memoriji(UPIS, 0, 'A')}});
    t.nit(0.0, {{Pristup_memoriji(UPIS, 1, 'B')}});
    t.nit(0.0, {{Pristup_memoriji(UPIS, 2, 'C')}});
    t.nit(0.0, {{Pristup_memoriji(UPIS, 3, 'D')}});
    // Četiri upisa, jedan za drugim, trajaće nešto više od 4 sekunde.

    // Nakon 3.5s istovremeno se pokreće 8 niti.
    // Namerno se pokreću dok prethodna grupa niti nije skroz završila, kako bi sve niti pokušale pristup memoriji
    // dok je memorija još zauzeta prethodnim operacijama.
    // Prve 4 niti pokušavaju da vrše upis u prve četiri lokacije.
    // Druge 4 niti pokušavaju da vrše čitanje iz tih lokacija.
    // Prema uslovima zadatka, trebalo bi da važi sledeće:
    // 1. Prednost će imati niti koje vrše čitanje, tako da će se očitati vrednosti A...D.
    // 2. Niti koje vrše čitanje će istovremeno izvršiti sva čitanja.
    // 3. Nakon njih startuju niti koje vrše upis i one moraju da se izvršavaju jedna po jedna.
    t.nit(3.5, {{Pristup_memoriji(UPIS, 0, 'E')}});
    t.nit(3.5, {{Pristup_memoriji(UPIS, 1, 'F')}});
    t.nit(3.5, {{Pristup_memoriji(UPIS, 2, 'G')}});
    t.nit(3.5, {{Pristup_memoriji(UPIS, 3, 'H')}});

    t.nit(3.5, {{Pristup_memoriji(CITANJE, 0)}});
    t.nit(3.5, {{Pristup_memoriji(CITANJE, 1)}});
    t.nit(3.5, {{Pristup_memoriji(CITANJE, 2)}});
    t.nit(3.5, {{Pristup_memoriji(CITANJE, 3)}});

    t.pokreni();
}

void testirajRazliciteMemorijskeReci() {
    cout << "Test operacija nad razlicitim memorijskim recima" << endl;
    cout << "************************************************" << endl;

    Dijagnostika d;
    Memorija mem(d, 20);
    Test t(mem);

    // Četiri niti upisuju vrednosti W...Z u adrese koje pripadaju različitim memorijskim rečima.
    // Prema uslovima zadatka, ovi upisi treba da se obave istovremeno:
    t.nit(0.0, {{Pristup_memoriji(UPIS, 0,  'W')}});
    t.nit(0.0, {{Pristup_memoriji(UPIS, 4,  'X')}});
    t.nit(0.0, {{Pristup_memoriji(UPIS, 8,  'Y')}});
    t.nit(0.0, {{Pristup_memoriji(UPIS, 12, 'Z')}});

    // Četiri niti čitaju vrednosti koje se upisane pokretanjem prethodne četiri niti.
    // Prema uslovima zadatka, ova čitanja treba da se obave istovremeno:
    t.nit(1.5, {{Pristup_memoriji(CITANJE, 0)}});
    t.nit(1.5, {{Pristup_memoriji(CITANJE, 4)}});
    t.nit(1.5, {{Pristup_memoriji(CITANJE, 8)}});
    t.nit(1.5, {{Pristup_memoriji(CITANJE, 12)}});

    t.pokreni();
}

int main() {
    testirajIstaMemorijskaRec();
    testirajRazliciteMemorijskeReci();

    return 0;
}

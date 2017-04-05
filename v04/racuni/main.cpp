/*
Napraviti program koji simulira prenos novca sa jednog bankovnog racuna na drugi.
Iznosi na racunima su predstavljeni datim nizom racuni.

Data funkcija transfer() predstavlja telo niti koje vrse prenos novca.
Funkcija 10 puta na slucajan nacin bira dva racuna i neki iznos novca i poziva funkciju prebaci() koja
skida novac sa prvog racuna i dodaje ga na drugi racun.
U funkciji prebaci(), nakon skidanja novca sa prvog racuna potrebno je jedna sekunda da
se novac uplati na drugi racun.
Povratna vrednost funkcije prebaci je struktura retVal koja sadrzi iznos na prvom racunu pre i
posle transakcije.

U glavnom programu potrebno je kreirati dve niti koje izvrsavaju funkciju transfer().
Ispisati ukupnu kolicinu novca na svim racunima u banci pre i posle transakcija.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
using namespace chrono;

#define UKUPNO_RACUNA 3

mutex m;

struct retVal{
    double staro;
    double novo;
};

double racuni[UKUPNO_RACUNA];

// Funckija koja prebacuje iznos sa izvornog na ciljni račun
retVal prebaci(int izvor, int cilj, double iznos) {
    // Zaključavanje resursa klasom unique_lock
    unique_lock<mutex> l(m);
    retVal ret;
    // Stara vrednost se čuva
    ret.staro = racuni[izvor];
    // Stanje računa se umanjuje za vrednost
    racuni[izvor] -= iznos;

    this_thread::sleep_for(seconds(1));

    // Povećanje računa za datu vrednost
    racuni[cilj] += iznos;
    ret.novo = racuni[izvor];

    return ret;
}

void transfer() {
    for (int i = 0; i < 10; i++) {
        int izvor = rand() % UKUPNO_RACUNA;
        int cilj = (izvor + 5) % UKUPNO_RACUNA;
        int iznos = rand() % 10 + 1;
        retVal r = prebaci(izvor, cilj, iznos);
        if ((r.staro - r.novo) != iznos)
            cout << "Prebaceno " << iznos << " sa racuna " << izvor << " na racun "
                << cilj << ". Na izvoru bilo " << r.staro << ", a ostalo " << r.novo << endl;
    }
}

// Pomoćna funkcija za proveru ukupnog stanja banke
double ukupno() {
    double s = 0;
    // Sumiranje svih računa
    for (int i = 0; i < UKUPNO_RACUNA; i++) {
        s += racuni[i];
    }

    return s;
}

int main() {
    // Inicijalizacija računa
    for (int i = 0; i < UKUPNO_RACUNA; i++) {
        racuni[i] = 10;
    }

    // Ispisuje početno stanje banke
    cout << "Na pocetku se u banci nalazi: " << ukupno() << " dinara." << endl;

    // Niti koje će vršiti transfere
    thread t1(transfer);
    thread t2(transfer);

    // Čekanje da se niti završi
    t1.join();
    t2.join();

    // Ispisuje krajnje stanje banke
    cout << "Nakon transakcija u banci se nalazi: " << ukupno() << " dinara." << endl;
    // Uvek isti, pošto su deljene promenljive dobro zaštićene

    return 0;
}

// Modelovati koriscenje racunara u racunarskoj ucionici. Broj racunara u ucionici se prosledjuje pri inicijalizaciji.
// Student pozivom operacije zauzmi() dolazi u ucionicu i zauzima prvi slobodan racunar, ukoliko takav racunar postoji.
// Ukoliko su svi racunari zauzeti, student mora sacekati da se neki racunar oslobodi.
// Operacija zauzmi() vraca redni broj racunara koji je student zauzeo.
// Student koristi racunar neki slucajan broj sekundi.
// Operacijom oslobodi(), student zavrsava rad u ucionici. Parametar metode je redni broj racunara koji student koristi i koji
// treba da se oslobi.
//
// Implementirati oznacene metode klase RC u fajlu rc.h

#include <thread>

#include "rc.h"

using namespace std;

void student(RC &rc, int br_indeksa) {
    int id_racunara = rc.zauzmi(br_indeksa); //zauzimanje racunara (uz moguce cekanje)
    this_thread::sleep_for(chrono::seconds(rand()%5 + 1)); //simuliranje koriscenja racunara uspavljivanjem niti
    rc.oslobodi(br_indeksa, id_racunara); //oslobadjanje racunara sa navedenim brojem
}

const int STUDENATA = 10;

void testirajSve() {
    Student s;
    RC rc(s, 3);
    thread t[STUDENATA];

    for (int i = 0; i < STUDENATA; ++i)
        t[i] = thread(student, ref(rc), i + 1);

    for (int i = 0; i < STUDENATA; ++i)
        t[i].join();
}

int main() {
    testirajSve();

    return 0;
}

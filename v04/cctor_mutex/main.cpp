/*
Data je klasa Brojac.
U main funkciji napraviti objekat klase Brojac. Ispis vrednosti Brojaca radi posebna nit.
Napraviti nit koja vrsi ispis i proslediti joj Brojac.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class Brojac {
        mutex m; //propusnica pripada klasi i obezbedjuje sprecavanje stetnog preplitanja pri vrsenju operacija nad objektima klase
        int broj; //vrednost Brojaca
    public:
        Brojac() : broj(0) {} //inicijalno je Brojac nula
        void inc() { unique_lock<mutex> l(m); ++broj; } //operacija povecava Brojac, ali pre toga zakljucava propusnicu i na taj nacin sprecava stetno preplitanja pri izmeni Brojaca
        void dec() { unique_lock<mutex> l(m); --broj; }//operacija smanjuje Brojac, ali pre toga zakljucava propusnicu i na taj nacin sprecava stetno preplitanja pri izmeni Brojaca
        friend ostream& operator<<(ostream& os, Brojac& b) { //preklopljen operator za ispis objekta klase Brojac. Ispisuje se atribut "broj" iz objekta
            unique_lock<mutex> l(b.m); //pre pristupa broju, zakljucava se propusnica da bi se sprecilo stetno preplitanje
            os << b.broj <<endl;
            return os;
        }
};

// Poziva ispis brojača
void f(Brojac& br) {
    cout << br << endl;
}

int main() {
    Brojac br;
    br.inc();
    // Pozivanje nove niti
    thread nit(f, ref(br));
    // Čekanje da se nit završi
    nit.join();
    return 0;
}

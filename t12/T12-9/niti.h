#ifndef NITI_H
#define NITI_H

#include <thread>

using namespace std;

double izracunaj(list<string> kupljeno, map<string, double> cenovnik) {
    double ukupno;

    thread t(obracunajUkupno, kupljeno, cenovnik, ref(ukupno));

    t.join();

    return ukupno;
}

#endif // NITI_H

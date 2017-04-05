#ifndef POVRATNA_H_INCLUDED
#define POVRATNA_H_INCLUDED

using namespace std;
using namespace chrono;

struct Povratna_vrednost {                      //Struktura koja opisuje povratnu vrednost, duzinu skoka ili bacanja i
    int rezultat;                               //trajanje skoka i bacanja u milisekundama.
    duration<double, milli> trajanje;
};

#endif // POVRATNA_H_INCLUDED

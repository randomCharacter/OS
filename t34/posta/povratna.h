#ifndef POVRATNA_H_INCLUDED
#define POVRATNA_H_INCLUDED

struct Povratna_vrednost { //povratna vrednost funkcije "uplati". Cuva podatke o tome koji je salter u pitanju i koliko je do tog trenutka na njemu uplaceno
    int salter;
    int uplaceno;
    Povratna_vrednost(int s, int u) : salter{s}, uplaceno{u} {}
};

#endif // POVRATNA_H_INCLUDED

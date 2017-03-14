#ifndef UCIONICA_H_INCLUDED
#define UCIONICA_H_INCLUDED

#include <string>
#include <vector>

#include "racunar.h"

using namespace std;

class Ucionica {
    private:
        string naziv;
        int kvadratura;
        vector<Racunar> racunari;
    public:
        Ucionica(string naziv, int kvadratura);

        void dodajRacunar(Racunar &r);

        string getNaziv();
        int getKvadratura();
        vector<Racunar> getRacunari();
};

#endif // UCIONICA_H_INCLUDED

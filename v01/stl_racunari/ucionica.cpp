#include "ucionica.h"

Ucionica::Ucionica(string naziv, int kvadratura) : naziv(naziv), kvadratura(kvadratura) {}

void Ucionica::dodajRacunar(Racunar &r) {
    racunari.push_back(r);
}

string Ucionica::getNaziv() {
    return naziv;
}

int Ucionica::getKvadratura() {
    return kvadratura;
}

vector<Racunar> Ucionica::getRacunari() {
    return racunari;
}

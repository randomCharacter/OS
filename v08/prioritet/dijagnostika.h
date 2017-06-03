#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;
public:
    // id_procesa   - ID procesa koji je upravo kreiran
    // prioritet    - prioritet procesa (0 - najviši prioritet)
    // broj_naredbi - broj naredbi koje proces treba da izvrši
    void proces_kreiran(int id_procesa, int prioritet, int broj_naredbi) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] Kreira se proces " << id_procesa << " prioriteta " << prioritet << " sa brojem naredbi " << broj_naredbi << endl;
    }

    // id_procesa   - ID procesa koji mora da čeka kako bi se procesor oslobodio
    void proces_ceka(int id_procesa) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] Proces " << id_procesa << " ceka da se oslobodi procesor." << endl;
    }

    // id_procesa   - ID procesa koji je završio izvršenje
    // prioritet    - prioritet procesa (0 - najviši prioritet)
    void proces_zavrsio(int id_procesa, int prioritet) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] Proces " << id_procesa << " sa prioritetom " << prioritet << " se zavrsio." << endl;
    }

    // id_procesa   - ID procesa koji je izvršio naredbu
    // naredba      - indeks naredbe koja je upravo zavrsena
    void izvrsio_naredbu(int id_procesa, int naredba) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " je izvrsio naredbu " << naredba << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

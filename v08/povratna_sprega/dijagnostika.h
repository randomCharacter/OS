#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;

public:
    // id_procesa   - ID procesa koji je upravo kreiran
    // broj_naredbi - broj naredbi koje proces treba da izvrši
    void proces_kreiran(int id_procesa, int broj_naredbi) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] Kreira se proces " << id_procesa << " sa brojem naredbi " << broj_naredbi << endl;
    }

    // id_procesa   - ID procesa koji mora da čeka kako bi se procesor oslobodio
    void proces_ceka(int id_procesa) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] Proces " << id_procesa << " ceka da se oslobodi procesor." << endl;
    }

    // id_procesa   - ID procesa koji je završio izvršenje
    void proces_zavrsio(int id_procesa) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " se zavrsio." << endl;
    }

    // id_procesa     - ID procesa koji je izvršio naredbu
    // naredba        - indeks naredbe koja je upravo zavrsena
    // novi_prioritet - novi nivo prioriteta dodeljen procesu nakon izvrsenja naredbe
    void izvrsio_naredbu(int id_procesa, int naredba, int novi_prioritet) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " je izvrsio naredbu " << naredba << " i dodeljen mu je novi prioritet: " << novi_prioritet << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

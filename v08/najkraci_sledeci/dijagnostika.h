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
    // trajanje     - trajanje procesa
    void proces_kreiran(int id_procesa, int trajanje) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] Kreira se proces " << id_procesa << " trajanja " << trajanje << "00 ms" << endl;
    }

    // id_procesa   - ID procesa koji mora da čeka kako bi se procesor oslobodio
    void proces_ceka(int id_procesa) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] Proces " << id_procesa << " ceka da se oslobodi procesor." << endl;
    }

    // id_procesa     - ID procesa koji se izvršio
    void proces_zavrsio(int id_procesa) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " je izvrsio svoje naredbe i zavrsio se." << endl;
    }

    void rasporedjivac_ceka() {
        unique_lock<mutex> l(term_mx);
        cout << "[RASPOREDJIVAC] Rasporedjivac ceka na signal da se aktivira." << endl;
    }

    // id_procesa   - ID procesa koji je raspoređivač izabrao za onaj koji će se sledeći izvršiti (-1 ako nema više procesa)
    void rasporedjivac_izvrsio(int id_procesa) {
        unique_lock<mutex> l(term_mx);
        cout << "[RASPOREDJIVAC] Rasporedjivac se izvrsio i zakazao izvrsenje procesa: " << id_procesa << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

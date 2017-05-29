#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;

public:
    // id_procesa - ID procesa čiji je zahtev prosleđen U/I uređaju
    // broj_staze - broj staze koji je potrebno obraditi (u sklopu ovog zahteva)
    void zahtev_prosledjen(int id_procesa, int broj_staze) {
        unique_lock<mutex> l(term_mx);
        cout << "[Proces] Proces " << id_procesa << " formirao je novi zahtev za pristup stazi: " << broj_staze << endl;
    }

    // id_procesa - ID procesa koji čeka kako bi njegov zahtev ka U/I uređaju bio obrađen
    // broj_staze - broj staze koji je potrebno obraditi (u sklopu ovog zahteva)
    void proces_ceka(int id_procesa, int broj_staze) {
        unique_lock<mutex> l(term_mx);
        cout << "[Proces] Proces " << id_procesa << " ceka kako bi pristupio stazi: " << broj_staze << endl;
    }

    // broj_staze - broj staze koji je upravo obrađen
    void zahtev_obradjen(int broj_staze) {
        unique_lock<mutex> l(term_mx);
        cout << "[U/I] obradjena je staza broj:  " << broj_staze << endl;
    }

    void uredjaj_aktiviran() {
        unique_lock<mutex> l(term_mx);
        cout << "[U/I] uredjaj je aktiviran i pocinje sa obradom zahteva" << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

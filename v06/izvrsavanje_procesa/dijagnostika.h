#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;
public:
    // Metode koje se ticu simulacije procesa

    // id         - id procesa
    // br_naredbi - broj naredbi koje proces treba da izvrsi
    void iniciran_proces(int id, int br_naredbi) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " je pokrenut i treba da izvrsi " << br_naredbi << " naredbi." << endl;
    }

    // id         - id procesa
    void proces_ceka(int id) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " ceka da se procesor oslobodi." << endl;
    }

    // id         - id procesa
    // naredba    - indeks naredbe koja se izvrsava (1..MAX)
    void proces_izvrsava(int id, int naredba) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " izvrsava svoju naredbu broj " << naredba << "." << endl;
    }

    // id         - id procesa
    // naredba    - indeks naredbe koja se izvrsava (1..MAX)
    void proces_zavrsio(int id, int naredba) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " je zavrsio svoju naredbu broj " << naredba << "." << endl;
    }


    // Metode koje se ticu obradjivaca U/I prekida

    void obradjivac_ceka() {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I OBRADJIVAC] ceka da se procesor oslobodi." << endl;
    }

    void obradjivac_izvrsava() {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I OBRADJIVAC] krece u obradu prekida." << endl;
    }

    void obradjivac_zavrsio() {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I OBRADJIVAC] je zavrsio obradu prekida." << endl;
    }


    // Metode koje se ticu U/I uredjaja

    void iniciran_prekid() {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I UREDJAJ] je postavio signal prekida." << endl;
    }

    void obradjen_prekid() {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I UREDJAJ] je ukinuo signal prekida." << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>
#include <vector>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;
public:
    // id_procesa    - id procesa koji čeka
    void proces_ceka(int id_procesa) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] " << id_procesa << ", ceka da se oslobodi dovoljno radne memorije kako bi mogao da se ucita i izvrsi." << endl;
    }

    // id_procesa    - id procesa koji se pokreće
    // br_lokacija   - broj memorijskih lokacija neophodnih procesu za rad
    void proces_krenuo(int id_procesa, int br_lokacija) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " sa " << br_lokacija << " stranica krenuo sa radom." << endl;
    }

    // id_procesa    - id procesa koji je zauzeo okvire
    // pocetak       - prvi okvir memorije koji je zauzet
    // kraj          - poslednji okvir memorije koji je zauzet
    void proces_zauzeo_okvire(int id_procesa, int pocetak, int kraj) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " zauzeo okvire od " << pocetak << " do " << kraj << endl;
    }

    // id_procesa    - id procesa koji se završio
    void proces_zavrsio(int id_procesa) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " se zavrsio." << endl;
    }

    void sazimanje_obavljeno() {
        unique_lock<mutex> l(term_mx);
        cout << "[SAZIMANJE] zavrseno." << endl;
    }

    // Ova metoda prikazuje zauzetost svih okvira u memoriji (indekse procesa koji drže svaki okvir), korišćenjem iteratora pocetak i kraj
    //
    // pocetak - iterator na prvi element kolekcije koja sadrži memorijske okvire
    // kraj    - iterator iza poslednjeg elementa kolekcije koja sadrži memorijske okvire
	void ispisi_memoriju(vector<int>::iterator pocetak, vector<int>::iterator kraj) {
        unique_lock<mutex> l(term_mx);
        cout << "[MEMORIJA] okviri: {";

        int c = 0;
        for (vector<int>::iterator it = pocetak; it != kraj; it++) {
            if (c++ > 0)
                cout << " ";
            cout << *it;
        }

        cout << "}" << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

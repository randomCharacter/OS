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
    // id_procesa    - id procesa koji se učitava
    // broj_stranica - koliko stranica memorije je potrebno procesu da bi mogao da se izvrši
    void proces_se_ucitava(int id_procesa, int broj_stranica) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] " << id_procesa << ", koji ima " << broj_stranica << " stranica, pokusava da se ucita u radnu memoriju..." << endl;
    }

    // id_procesa    - id procesa koji se učitava
    void proces_ceka(int id_procesa) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] " << id_procesa << ", ceka da se oslobodi dovoljno radne memorije." << endl;
    }

    // id_procesa    - id procesa koji je učitan i kreće u izvršenje
    void proces_se_izvrsava(int id_procesa) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] " << id_procesa << " se ucitao u radnu memoriju i krece sa izvrsenjem. Okviri u memoriji: ";
		cout << endl;
    }

    // id_procesa    - id procesa koji se izvršio
    void proces_se_zavrsio(int id_procesa) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] " << id_procesa << " se zavrsio, suspenduje se..." << endl;
	}

    // Ova metoda prikazuje sve indekse okvira koji su postavljeni u kolekciju kojoj se pristupa pomoću iteratora pocetak i kraj
    //
    // pocetak - iterator na prvi element kolekcije koja sadrži indekse okvira koje treba ispisati
    // kraj    - iterator iza poslednjeg elementa kolekcije koja sadrži indekse okvira koje treba ispisati
	void ispisi_okvire(vector<int>::iterator pocetak, vector<int>::iterator kraj) {
        unique_lock<mutex> l(term_mx);
        cout << "{";

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

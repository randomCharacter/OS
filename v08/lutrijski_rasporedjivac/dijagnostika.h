#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;

    // Pomoćna metoda koja ispisuje vrednost tiketa sadržanih u nekom kontejneru
    //
    // pocetak - iterator na prvi element kontejnera koji sadrži tikete
    // kraj    - iterator iza poslednjeg elementa kontejnera koji sadrži tikete
    void ispisi_tikete(vector<int>::const_iterator pocetak, vector<int>::const_iterator kraj) {
        for (auto it = pocetak; it != kraj; it++)
            cout << *it << " ";
    }

public:
    // id_procesa   - ID procesa koji je upravo kreiran
    // broj_naredbi - broj naredbi koje proces treba da izvrši
    // pocetak      - iterator na prvi element kontejnera koji sadrži tikete
    // kraj         - iterator iza poslednjeg elementa kontejnera koji sadrži tikete
    void proces_kreiran(int id_procesa, int broj_naredbi, vector<int>::const_iterator pocetak, vector<int>::const_iterator kraj) {
		unique_lock<mutex> l(term_mx);
		cout << "[PROCES] Kreira se proces " << id_procesa << " sa brojem naredbi " << broj_naredbi << " i sa tiketima: " << endl;
		ispisi_tikete(pocetak, kraj);
		cout << endl;
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
    // sledeci_proces - ID procesa koji će se sledeći izvršavati
    void izvrsio_naredbu(int id_procesa, int naredba, int sledeci_proces) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCES] " << id_procesa << " je izvrsio naredbu " << naredba << ". Sledeci izabrani proces je: " << sledeci_proces << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

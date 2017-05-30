#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;
public:
    // Metode koje se odnose na simulaciju procesa

    // id - ID procesa
    void proces_ceka(int id) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " se uvezuje u red spremnih procesa." << endl;
    }

    // id - ID procesa
    void proces_izvrsio_standardnu(int id) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " izvrsio standardnu instrukciju." << endl;
    }

    // id - ID procesa
    void proces_ceka_na_UI(int id) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " odlazi u cekanje zbog U/I operacije." << endl;
    }

    // id - ID procesa
    void proces_zavrsio_UI(int id) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " probudjen iz cekanja od strane U/I uredjaja " << endl;
    }

    // id - ID procesa
    void proces_oslobadja_procesor(int id) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " oslobadja procesor." << endl;
    }

    // id - ID procesa
    void proces_zavrsio(int id) {
        unique_lock<mutex>l (term_mx);
        cout << "[PROCES] " << id << " se zavrsio." << endl;
    }


    // Metode koje se odnose na simulaciju U/I podsistema:

    void ui_ceka() {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I] podsistem ceka na zahtev." << endl;
    }

    // id_procesa - ID procesa čiji se zahtev obrađuje
    void ui_zapocinje(int id_procesa) {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I] podsistem obradjuje zahtev procesa " << id_procesa << endl;
    }

    // id_procesa - ID procesa čiji je zahtev obrađen
    void ui_zavrsio(int id_procesa) {
        unique_lock<mutex>l (term_mx);
        cout << "[U/I] podsistem je obradio zahtev procesa " << id_procesa << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

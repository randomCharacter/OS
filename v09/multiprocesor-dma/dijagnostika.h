#ifndef DIJAGNOSTIKA_H_INCLUDED
#define DIJAGNOSTIKA_H_INCLUDED

#include <iostream>
#include <mutex>

using namespace std;

class Dijagnostika {
private:
    mutex term_mx;

public:
    // id_procesora - ID procesora koji je učitao vrednost iz memorije
    // adresa       - adresa sa koje je očitana vrednost
    // vrednost     - vrednost koja je pročitana iz memorije
    void procesor_ucitao_iz_memorije(int id_procesora, int adresa, char vrednost) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCESOR] Procesor " << id_procesora << " je procitao sa adrese " << adresa << " vrednost = " << vrednost << endl;
    }

    // id_procesora - ID procesora koji je upisao vrednost u memoriju
    // adresa       - adresa na koju je upisana vrednost
    // vrednost     - vrednost koja je upisana u memoriju
    void procesor_upisao_u_memoriju(int id_procesora, int adresa, char vrednost) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCESOR] Processor " << id_procesora << " je zapisao na adresu " << adresa << " vrednost = " << vrednost << endl;
    }

    // id_procesora - ID procesora koji je zatražio DMA transfer
    void procesor_zahteva_dma(int id_procesora) {
        unique_lock<mutex> l(term_mx);
        cout << "[PROCESOR] Processor " << id_procesora << " zahteva DMA transfer" << endl;
    }

    // odakle - adresa u memoriji odakle počinje DMA transfer
    // koliko - koliko karaktera se kopira iz memorije
    // kome   - destinaciona adresa - na nju se upisuju očitani karakteri
    void dma_transfer_obavljen(int odakle, int koliko, int kome) {
        unique_lock<mutex> l(term_mx);
        cout << "[DMA] Transfer obavljen: odakle = " << odakle << ", koliko = " << koliko << ", kome = " << kome << endl;
    }
};

#endif // DIJAGNOSTIKA_H_INCLUDED

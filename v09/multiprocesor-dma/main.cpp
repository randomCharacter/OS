// Napisati konkurentni program koji modeluje ponašanje računarskog sistema koji ima sledeće delove:
// - magistralu - sabirnicu,
// - N procesora,
// - memoriju,
// - DMA kontroler.
// Svi procesori, memorija i DMA kontroler su zakačeni na istu magistralu.
//
// Memorija je predstavljena klasom Memorija. Pri instanciranju objekta ovog tipa se zadaje veličina memorije. Memorija se može čitati (metodom citaj()) i pisati (metodom pisi()). Obe operacije treba da budu thread-safe. I čitanje iz memorije i pisanje u memoriju traje po 300 ms.
//
// Magistrala je predstavljena klasom Magistrala. Magistrala ima 3 metode koje pozivaju procesori (citaj_memoriju(), pisi_u_memoriju(), dma()) i jednu koju poziva DMA kontroler (okidac_dma_kontrolera()). Sve operacije treba da budu thread-safe. Pre pristupa memoriji neophodno je zaključati (ekskluzivno) magistralu. Zaključavanje magistrale traje 700ms. Po završetku pristupa magistrala se otključava (trenutno). DMA transfer se obavlja tako što se magistrala zaključa jednom, obave se svi transferi i na kraju se magistrala otključa.
//
// DMA kontroler je predstavljen funkcijom DMA_kontroler. Kontroler poziva operaciju Magistrala::okidac_dma_kontrolera() i u njoj čeka dok neki procesor ne zatraži DMA prenos.
//
// Procesori pri stvaranju dobiju zadat program (u obliku vektora naredbi) koji treba da izvrše. Naredbe su predstavljene objektima klase Naredba. Moguće naredbe su:
// - pisanja u memoriju,
// - čitanje iz memorije i
// - DMA transfer
//
// Stvoriti nekoliko niti koje pokazuju da program radi ispravno.
//
// Dato je nekoliko programa (u funkciji main()) za primer, zarad lakšeg testiranja.
//

#include <vector>
#include <thread>

#include "memorija.h"
#include "magistrala.h"
#include "naredba.h"

using namespace std;

// Funkcija niti koju izvršavaju niti koje simuliraju procesore:
void procesor(Magistrala& magistrala, vector<Naredba>& program, int idp) {
    for(auto it = program.begin(); it != program.end(); it++) {
        if(it->tip_transfera == Magistrala::MEM_CITAJ) {
            it->vrednost = magistrala.citaj_memoriju(it->odakle);
            magistrala.getDijagnostika().procesor_ucitao_iz_memorije(idp, it->odakle, it->vrednost);
        } else if(it->tip_transfera == Magistrala::MEM_PISI) {
            magistrala.pisi_u_memoriju(it->kome, it->vrednost);
            magistrala.getDijagnostika().procesor_upisao_u_memoriju(idp, it->kome, it->vrednost);
        } else {
            Magistrala::DMA_transfer transfer = {it->odakle, it->koliko, it->kome};
            magistrala.dma(transfer);
            magistrala.getDijagnostika().procesor_zahteva_dma(idp);
        }
    }
}

// Funkcija niti koja simulira DMA kontroler:
void DMA_kontroler(Magistrala& magistrala) {
    while (true) {
        Magistrala::DMA_transfer transfer = magistrala.okidac_dma_kontrolera();
        magistrala.getDijagnostika().dma_transfer_obavljen(transfer.odakle, transfer.koliko, transfer.kome);
    }
}

void testirajSve() {
    vector<Naredba> I  = {
        {0, '0'},   // Upis
        {4, '4'},   // Upis
        {0},        // Čitanje
        {8, '8'}    // Upis
    };
    vector<Naredba> II = {
        {1, '1'},   // Upis
        {3, '3'},   // Upis
        {0, 5, 10}, // DMA transfer
        {10},       // Čitanje
        {11}        // Čitanje
    };
    vector<Naredba> III = {
        {8, '8'},   // Upis
        {9, '9'},   // Upis
        {2, '2'},   // Upis
        {12},       // Čitanje
        {13},       // Čitanje
        {14}        // Čitanje
    };

    Dijagnostika d;
    Memorija mem(16);
    Magistrala magistrala(d, mem);

    thread dma_kontr(DMA_kontroler, ref(magistrala));
    dma_kontr.detach();

    thread P1(procesor, ref(magistrala), ref(I), 1);
    thread P2(procesor, ref(magistrala), ref(II), 2);
    thread P3(procesor, ref(magistrala), ref(III), 3);
    P1.join();
    P2.join();
    P3.join();
}

int main() {
    testirajSve();

    exit(0);  //exit 0 - predstavlja signal detach-ovanoj niti da prekine rad jer više nije neophodna
}

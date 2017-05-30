// Napraviti konkurentni program koji simulira izvršavanje procesa u računarskom sistemu sa jednim ulazno-izlaznim uređajem.
//
// Procesi su predstavljeni nitima. Telo niti je data funkcija proces. Svaki proces izvršava određeni broj naredbi pozivom funkcije izvrsi_proces. Naredbe se izvršavaju jedna po jedna. Da bi proces izvršio naredbu, mora da dobije procesor. Nakon izvršavanja jedne naredbe, proces odlazi u cooldown period od 100 ms, pre nego što ponovo pokuša da koristi procesor i izvrši narednu naredbu. Izvršavanje jedne naredbe traje 1 sekundu.
//
// Pored procesa i ulazno-izlazni uređaj koristi procesor. Smer komunikacije je takav da se ulazno-izlazni uređaj obraća operativnom sistemu kako bi ga informisao o novom ulazu (npr. tastatura obaveštava OS o pritisnutom tasteru). Ovo se postiže time što ulazno-izlazni uređaj postavlja prekid kojim signalizira operativnom sistemu da ima novi podatak i da ima potrebu da izvrši kod koji obrađuje taj podatak na procesoru. U ovoj simulaciji, svakih nekoliko sekundi, uređaj postavlja prekid i traži njegovu obradu od procesora pozivom funkcije "prekini". Obrada prekida mora da sačeka da procesor postane raspoloživ. Ukoliko i procesi i uređaj čekaju na procesor, prednost u dobijanju procesora ima uređaj, jer je obrada prekida prioritet u odnosu na izvršavanje naredbi procesa. Sama obrada prekida na procesoru traje 300 ms.
//
// Implementirati označene metode, konstruktor i dodati privatne članove u klasu Procesor (fajl procesor.h).

#include <thread>

#include "procesor.h"

using namespace std;
using namespace chrono;

// Funkcija niti koja simulira izvrsenje procesa
void proces(Procesor& p, int id, int br_naredbi) {
    p.getDijagnostika().iniciran_proces(id, br_naredbi);
    p.izvrsi_proces(id, br_naredbi);
}

// Funkcija niti koja simulira ulazno/izlazni podsistem
void ui_uredjaj(Procesor& p) {
    while (true) {
        this_thread::sleep_for(seconds(rand()%5+1));
        p.getDijagnostika().iniciran_prekid();
        p.prekini();
        p.getDijagnostika().obradjen_prekid();
    }
}

const int BR_PROCESA = 5;
const int MAKS_NAREDBI = 10;

void testirajSve() {
    Dijagnostika d;
    Procesor p(d);

    // Pokretanje niti koja simulira ulazno/izlazni podsistem:
    thread ui(ui_uredjaj, ref(p));
    ui.detach();

    // Pokretanje niti koje simuliraju procese:
    thread procesi[BR_PROCESA];
    for (int i = 0; i < BR_PROCESA; i++) {
        procesi[i] = thread(proces, ref(p), i+1, rand()%MAKS_NAREDBI+1);
    }

    for (int i = 0; i < BR_PROCESA; i++)
        procesi[i].join();
}

int main() {
    testirajSve();

    exit(0);  //exit 0 - predstavlja signal detach-ovanoj niti da prekine rad jer više nije neophodna
}

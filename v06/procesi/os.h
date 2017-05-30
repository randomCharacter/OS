#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <queue>

#include "dijagnostika.h"
#include "proces.h"

using namespace std;
using namespace chrono;

struct zahtev {
    condition_variable cv;
    int id_procesa;
    bool gotov;
    zahtev(int id) : id_procesa(id) {
        gotov = false;
    }
};

class OS {
private:
    Dijagnostika& dijagnostika;
    mutex m;
    condition_variable cv_spremni, cv_ui;
    int max_i;
    int aktivan;
    queue<zahtev*> zahtevi; // Red zahteva (mora zahtev*, a ne zahtev zato što condition_variable ne može da se kopira)

public:
    // d                 - Referenca na objekat Dijagnostika, koji služi za ispis stanja simulacije
    // maks_uzastopnih_i - Maksimalan dozvoljen broj uzastopnih STANDARD instrukcija, nakon kojeg obavezno sledi preključivanje
    OS(Dijagnostika& d, int maks_uzastopnih_i) : dijagnostika(d), max_i(maks_uzastopnih_i) {
        aktivan = -1;
    }

    // Pomoćna metoda koja vraća objekat klase Dijagnostika koji služi za ispis stanja simulacije
    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira proces kako bi se izvršile sve instrukcije procesa.
    //
    // p - Struktura koja sadrži opis procesa (ID procesa i instrukcije koje treba izvršiti)
    //
    // Ukoliko proces ne može da se izvrši (procesor je zauzet), proces mora da se uveže u red spremnih i treba pozvati metodu dijagnostika.proces_ceka.
    // Ukoliko dođe do toga da izvršenje procesa prestaje (dostignut je maksimalan dozvoljen broj uzastopnih STANDARD instrukcija ili su sve instrukcije procesa izvršene), potrebno je pozvati dijagnostika.proces_oslobadja_procesor.
    // Kada se izvrši STANDARD instrukcija nekog procesa, potrebno je pozvati dijagnostika.proces_izvrsio_standardnu.
    // Kada se izvršava UI instrukcija, potrebno je pozvati par metoda: dijagnostika.proces_ceka_na_UI kada se pokrene U/I operacija (nakon čega sledi čekanje na U/I podsistem) i dijagnostika.proces_zavrsio_UI kada se završi U/I operacija.
	void izvrsi_proces(Proces& p) {
        unique_lock<mutex> l(m);
        // Broj izvršenih instrukcija
        int ins = 0;
        for (auto it = p.instrukcije.begin(); it != p.instrukcije.end(); it++) {
            // Ukoliko je je procesor zauzet od strane drugog procesora
            while (aktivan != p.id && aktivan != -1) {
                // Ide na čekanje
                dijagnostika.proces_ceka(p.id);
                cv_spremni.wait(l);
            }
            // Zauzimanje procesora
            aktivan = p.id;

            // Izvršavanje u zavisnosti od tipa procesa
            if (*it == STANDARD) {
                // Izvršavanje
                l.unlock();
                this_thread::sleep_for(milliseconds(rand() % 40 + 10));
                l.lock();
                dijagnostika.proces_izvrsio_standardnu(p.id);
                // Ukoliko je izvršeno maksimalno dozvoljeno naredbi ili nema više instrukcija
                if (++ins >= max_i || it == p.instrukcije.end() - 1) {
                    dijagnostika.proces_oslobadja_procesor(p.id);
                    ins = 0;
                    // Oslobađa se procesor
                    aktivan = -1;
                    // Obaveštava se sledeći proces
                    cv_spremni.notify_one();
                    // Cooldown period
                    l.unlock();
                    this_thread::sleep_for(milliseconds(100));
                    l.lock();
                }
            } else {
                // Kreiranje novog zahteva i dodavanje u list
                zahtev* z = new zahtev(p.id);
                zahtevi.push(z);
                // Obrađivač se obaveštava da ima zahteva u listi
                cv_ui.notify_one();
                // Čekanje da se zahtev obradi
                dijagnostika.proces_ceka_na_UI(p.id);
                while (!z->gotov) {
                    // Proces ide na čekanje i sledeći spreman se obaveštava
                    ins = 0;
                    aktivan = -1;
                    cv_spremni.notify_one();
                    z->cv.wait(l);
                }
                // Kada završi
                dijagnostika.proces_zavrsio_UI(p.id);
                delete z;
            }
        }
	}

    // Metoda koju poziva nit koja simulira obrađivač U/I zahteva kako bi se obradili trenutno pohranjeni U/I zahtevi.
    //
    // Potrebno je pozvati dijagnostika.ui_ceka kada ne postoje trenutno pohranjeni U/I zahtevi i obrađivač čeka na prvi.
    // Potrebno je pozvati dijagnostika.ui_zapocinje kada obrađivač krene u obradu U/I zahteva. Kada obrađivač završi taj zahtev, potrebno je pozvati dijagnostika.ui_zavrsio.
	void obradi_zahteve() {
        unique_lock<mutex> l(m);
        // Obrađivač stalno radi u pozadini
        while (true) {
            // Ako nema zahteva ide na čekanje
            while (zahtevi.empty()) {
                dijagnostika.ui_ceka();
                cv_ui.wait(l);
            }
            // Kada se pojavi neki vadi se iz reda i obrađuje se
            zahtev *z = zahtevi.front();
            zahtevi.pop();
            // Početak obrade
            dijagnostika.ui_zapocinje(z->id_procesa);
            // Obrada
            l.unlock();
            this_thread::sleep_for(milliseconds(rand() % 200 + 100));
            l.lock();
            // Kraj obrade
            z->gotov = true;
            dijagnostika.ui_zavrsio(z->id_procesa);
            z->cv.notify_one();
        }
	}
};

#endif // OS_H_INCLUDED

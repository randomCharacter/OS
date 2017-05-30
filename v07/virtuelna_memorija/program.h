#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <string>
#include <mutex>
#include <condition_variable>

#include "dijagnostika.h"
#include "naredba.h"
#include "povratna_vrednost.h"

using namespace std;
using namespace chrono;

class Program {
private:
    Dijagnostika& dijagnostika;
    int slobodno_ram;           // Količina slobodne RAM memorije
    int zauzeto_virtuelna;      // Količina zauzete virtuelne memorije. Ova simulacija smatra da je VM neograničenog kapaciteta.

    mutex m;

    bool aktiviraj_zauzimanje_VM;       // Zahteva se zauzimanje VM od strane OS.
    condition_variable virtuelna_start; // Signal kojim se OS obaveštava da je potrebno da zauzme VM.
    int zauzmi_virtuelne;               // Koliko VM je potrebno zauzeti.

    condition_variable virtuelna_stop;  // Ovim signalom OS obaveštava program da je zauzimanje VM završeno.

    // Pomoća metoda koja izvršava alokaciju memorije
    //
    // kolicina_memorije - koliko memorije je potrebno zauzeti
    void izvrsi_malloc(int kolicina_memorije) {
        unique_lock<mutex> l(m);
        if (slobodno_ram < kolicina_memorije) {     // Ukoliko ima manje RAM-a nego što je zahtevano
            aktiviraj_zauzimanje_VM = true;         // aktivira se zauzimanje VM
            virtuelna_start.notify_one();           // pored setovanja bool varijable, notifikuje se CV
            zauzmi_virtuelne = kolicina_memorije;   // u polju "zauzmi_virtuelne" OS očekuje da se postavi koliko VM je potrebno
            while (aktiviraj_zauzimanje_VM)         // OS će postaviti polje aktiviraj_zauzimanje_VM na false kada završi
                virtuelna_stop.wait(l);
        } else
            slobodno_ram -= kolicina_memorije;      // Ako ima dovoljno RAM-a, on se zauzima

        this_thread::sleep_for(seconds(1));         //sleep (ako ima višestruki malloc).
    }

public:
    Program(Dijagnostika& d, int kapacitet) : dijagnostika(d) {
        slobodno_ram = kapacitet;
        zauzeto_virtuelna = 0;
        aktiviraj_zauzimanje_VM = false;
    }

    Dijagnostika& getDijagnostika() {
        return dijagnostika;
    }

    // Metoda koju poziva nit koja simulira izvršenje programa, kako bi zauzela potrebnu memoriju
    //
    // naredba - naredba koja se izvršava (naredba kojom se zauzima memorija)
    // Vraća instancu klase Povratna_vrednost koja sadrži opis zauzeća memorije NAKON izvršenja naredbe.
    Povratna_vrednost izvrsi_naredbu(Naredba naredba) {
        if (naredba.tip == "repeat") {       // Ukoliko je tip naredbe repeat.
            for (int i = 0; i < naredba.ponavljanja; ++i)       // operacija malloc se ponavlja zadat broj puta
                izvrsi_malloc(naredba.kolicina_memorije);
        } else {
            izvrsi_malloc(naredba.kolicina_memorije);          // U suprotnom, jednom se izvrši malloc.
        }
        return {slobodno_ram, zauzeto_virtuelna};       // Nakon zauzimanja memorije vraća se koliko ima slobodnog RAM-a i
                                                        // koliko je zauzeto virtuelne memorije.
    }

    // Metoda koju poziva nit koja simulira deo operativnog sistema koji se bavi zauzimanjem virtuelne memorije kako bi se zauzela određena količina VM
    // Potrebnu količinu VM (kao i bilo koju drugu neophodnu informaciju) preneti u nekom izabranom polju klase.
    int zauzmi_virtuelnu_memoriju() {
        unique_lock<mutex> l(m);

        while (!aktiviraj_zauzimanje_VM) {      // Dok god nema aktiviranja OS, čeka se komanda
             virtuelna_start.wait(l);
        }

        this_thread::sleep_for(seconds(1));     // Zauzimanje virtuelne memorije traje 1 sekund.

        zauzeto_virtuelna += zauzmi_virtuelne;  // Povećaj količinu zauzete virtuelne memorije.
        aktiviraj_zauzimanje_VM = false;        // Pozivaoc očekuje da se ovo polje vrati na "false" kada je operacija završena.
        virtuelna_stop.notify_one();            // Javi malloc-u da je zauzeta virtuelna memorija.

        return zauzeto_virtuelna;
    }
};

#endif // PROGRAM_H_INCLUDED

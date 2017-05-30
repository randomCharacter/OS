// Napisati konkurentni program koji simulira menjanje kapaciteta raspoložive memorije pri izvršavanju nekog programa (uvođenjem virtuelne memorije).
// Program je predstavljen klasom Program. Konstruktoru klase prosleđuje se ukupni kapacitet RAM memorije. Nit predstavljena datom funkcijom "izvrsavanje" izvršava listu naredbi programa pozivom metode izvrsi_naredbu() klase Program. Metoda izvrsi_naredbu() izvršava prosleđenu naredbu. Moguće naredbe su:
// - malloc(X) - zauzima se X količina memorije i
// - repeat[N]malloc(X) - N puta se zauzima količina memorije X
//
// Pri zauzimanju memorije, pokušava se zauzeti RAM memorija. Ukoliko nema dovoljno RAM memorije, vrši se zauzimanje virtuelne memorije. U ovoj simulaciji kapacitet virtuelne memorije je neograničen.
//
// Za zauzimanje virtuelne memorije zadužen je operativni sistem predstavljen posebnom niti koja izvršava datu funkciju "os". Funkcija "os" poziva metodu zauzmi_virtuelnu_memoriju() klase program. Nit vrši zauzimanje virtuelne memorije onda kada dobije zahtev od niti koja vrši izvršavanje naredbi programa. Kao rezultat, metoda zauzmi_virtuelnu_memoriju() vraća podatak koliko je ukupno virtuelne memorije trenutno zauzeto.
//
// Bilo kakav pristup memoriji (i RAM i virtuelnoj) je ekskluzivan - u jednom trenutku samo jedna naredba može zauzimati memoriju i dok operativni sistem zauzima virtuelnu memoriju, nije moguć pristup bilo kojoj memoriji.
//

#include <thread>
#include <vector>

#include "program.h"

using namespace std;
using namespace chrono;

// Funkcija koju koriste niti koje simuliraju izvršenje programa:
void izvrsavanje(Program &p, vector<Naredba>& naredbe) {
    for (vector<Naredba>::iterator it = naredbe.begin(); it != naredbe.end(); ++it) {
        p.getDijagnostika().pokrenuta_naredba(*it);
        Povratna_vrednost ret = p.izvrsi_naredbu(*it);
        p.getDijagnostika().izvrsena_naredba(*it, ret.slobodno_ram, ret.zauzeto_u_virtuelnoj);
    }
}

// Funkcija koju koristi nit koja simulira servis operativnog sistema za zauzimanje (alokaciju) virtuelne memorije:
void os(Program& p) {
    while (true) {
        int ret = p.zauzmi_virtuelnu_memoriju();
        p.getDijagnostika().izvrseno_zauzimanje_vm(ret);
    }
}

void testirajSve() {
    Dijagnostika d;
    Program p(d, 100);      // Ima 100 MB RAM memorije
    vector<Naredba> vektor_naredbi;

    Naredba n1("malloc", 1, 70);
    Naredba n2("malloc", 1, 50);
    Naredba n3("malloc", 1, 30);
    Naredba n4("repeat", 3, 10);

    vektor_naredbi.push_back(n1);
    vektor_naredbi.push_back(n2);
    vektor_naredbi.push_back(n3);
    vektor_naredbi.push_back(n4);

    thread i(izvrsavanje, ref(p), ref(vektor_naredbi));
    thread o(os, ref(p));
    i.join();
    o.detach();
}

int main() {
    testirajSve();

    exit(0);   //exit 0 - predstavlja signal detach-ovanoj niti da prekine rad jer više nije neophodna
}

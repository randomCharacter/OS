/*
    Napraviti program koji ispituje ispravnost jednog od Marfijevih zakona, koji glasi:
    "Vas red je uvek najsporiji.".

    Simulirati placanje robe na tri kase. Rad svake kase predstavljen je jednom niti.
    Svaka nit dobija parametar koji predstavlja broj kupaca koje kasa treba da opsluzi.
    Broj kupaca je slucajan broj izmedju 1 i 30.
    Opsluzivanje svakog kupca simulirati uspavljivanjem niti na slucajan vremenski period izmedju 1 i 100 ms.

    Izracunati trajanje kupovine na svakoj od kasa.

    Na pocetku programa potrebno je pitati korisnika da unese redni broj kase na
    kojoj zeli da izvrsi kupovinu.
    Na kraju ispisati da li je Marfijev zakon potvrdjen.
    Ukoliko je kupovina na kasi koju je kupac izabrao trajala najduze, tada je Marfijev zakon potvrdjen.
*/

#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;

// Broj kasa
const int N = 3;
// Maksimalno vreme cekanja
const int MAX_CEKANJE = 100;
// Maksimalan broj ljudi u redu
const int MAX_LJUDI = 30;

// Funkcija koja simulira cekanje u redu sa n kupaca
void f(int n, duration<double, milli> &v) {
    // Uzimanje vremena pre početka izvršavanja
    auto t0 = system_clock::now();

    // Simuliranje čekanja
    for (int i = 0; i < n; i++) {
        // Uspavljuje trenutnu nit za prosledjeno vreme
        this_thread::sleep_for(milliseconds(rand() % MAX_CEKANJE + 1));
    }

    // Uzimanje vremena nakon završetka izvršavanja
    auto t1 = system_clock::now();

    // Ukupno čekanje
    v = t1 - t0;
}

int main() {
    // Slucajni brojevi se racunaju u odnosu na trnutno vreme
    srand(time(NULL));

    // Redni broj kase
    int n;
    cout << "Unesite redni broj kase [0<=n<3]: " << endl;
    cin >> n;

    // Vremena čekanja na svakoj kasi
    duration<double, milli> v[N];

    // Niti za simulaciju čekanja
    thread t[N];

    //Simulacija čekanja
    for (int i = 0; i < N; i++) {
        t[i] = thread(f, rand() & MAX_LJUDI + 1, ref(v[i]));
    }
    for (int i = 0; i < N; i++) {
        t[i].join();
    }

    // Pronalaženje reda u kojem se najduže čekalo
    auto max = v[0];
    for (int i = 0; i < N; i++) {
        cout << "Ukupno cekanje reda br. " << i << " je: " << v[i].count() << endl;
        // Pronalaženje maksimuma
        if (v[i] > max) {
            max = v[i];
        }
    }

    // Provera zakona
    if (v[n] == max) {
        cout << "Marfijev zakon potvrdjen!" << endl;
    } else {
        cout << "Marfijev zakon nije potvrdjen!" << endl;
    }
}

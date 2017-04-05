/*
Napisati konkurentni program koji stvara 2 niti od funkcije f. Funkcija f treba da
prolazi kroz petlju duzine 10000 elemenata i ispisuje parne ili neparne brojeve (od tih
10000 elemenata).

Ispis parnih ili neparnih brojeva se vrsi pozivom metoda ispisi_parne i ispisi_neparne.
U metodi ispisi_parne prvo se pre ispisa zakljucava muteks m1, a potom muteks m2.
U metodi ispisi_neparne prvo se pre ispisa zakljucava muteks m2, a potom muteks m1.

Sva zakljucavanja se vrse kroz objekte klase unique_lock.

Pratiti izvrsavanje programa.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

const int N = 10000;

// 2 mutexa
mutex m1, m2;

void ispisi_parne() {
    for (int i = 0; i < N; i += 2) {
        // U konstruktoru poziva m1.lock, a u destruktoru m1.unlock, čime smanjuje prostor za grešku zbog neoslobođenog resursa
        unique_lock<mutex> l1(m1);
        unique_lock<mutex> l2(m2);
        cout << i << " ";
    }
}

void ispisi_neparne() {
    for (int i = 1; i < N; i += 2) {
        unique_lock<mutex> l2(m2);
        unique_lock<mutex> l1(m1);
        cout << i << " ";
    }
}

int main() {
    // Pozivanje iz niti
    thread t1(ispisi_neparne);
    thread t2(ispisi_parne);

    // Čekanje da se niti završe
    t1.join();
    t2.join();
    // Verovatno se nikad neće završiti zbog deadlock-a
    // jedna nit će zatvoriti m1, druga m2, i resurs na koji čekaju se nikad neće osloboditi

    return 0;
}

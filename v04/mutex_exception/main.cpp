/*
Napraviti konkurentni program u kom se u funkciji niti pravi veliki niz od 1000000000000 elemenata.
Niz se pravi dinamicki. Kreiranje niza zastititi try - catch blokom. U okviru try catch bloka zakljucati
mutex pre pravljenja niza i otkljucati ga nakon pravljenja niza. Posmatrati ponasanje programa.

Nakon toga promeniti kod tako da se ne zakljucava mutex eksplicitno vec da se koristi klasa unique_lock.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

const int N = 1000000000000;

mutex m;

void f() {
    int *niz;
    try {
        unique_lock<mutex> l(m);
        //m.lock();
        niz = new int[1000000000000];
        //m.unlock();
    } catch (const exception e) {
        // Ispis greške
        cout << "Neuspesna alokacija!: " << e.what() << endl;
    }
}

int main() {
    // Kreiranje niti
    thread t1(f);
    thread t2(f);

    // Čekanje da se niti završe
    t1.join();
    t2.join();

    // Ukoliko se koristi mutex, nakon pojave izuzetka
    // neće se pozvati m.unlock, te će resurs ostati zaključan
    // Ukoliko se koristi unique_lock, nakon pojave izuzetka
    // pozvaće se destruktor, samim tim i unique_lock, te će resurs biti otključan

    return 0;
}

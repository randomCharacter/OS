/*
    Prosiriti zadatak najbilzi_nuli_vreme.cpp ispisom u sekundama i minutima.
*/

#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

using namespace std;
using namespace chrono;

// Broj elemenata niza
const int N = 900000;
// Broj niti
const int BROJ_NITI = 3;

// Funkcija koja traži minimum
void f(vector<double>::const_iterator pocetak, vector<double>::const_iterator kraj, double &min, duration<double, milli> &v) {
    // Uzimanje vremena pre početka izvršavanja
    system_clock::time_point t0 = system_clock::now();
    // Traženje minimuma
    min = *pocetak;
    for (auto it = pocetak + 1; it != kraj; it++) {
        if (abs(*it) < abs(min)) {
            min = *it;
        }
    }
    // Uzimanje vremena nakon završetka izvršavanja
    system_clock::time_point t1 = system_clock::now();

    // Ukupno trajanje je razlika vremena kraja i početka
    v = t1 - t0;
}

int main() {
    // Slucajni brojevi se racunaju u odnosu na trnutno vreme
    srand(time(NULL));

    vector<double> A(N);

    // Popunjavanje vektora slučajnim brojevima
    for (int i = 0; i < N; i++) {
        A[i] = rand();
    }

    // Potrebne niti
    thread t[BROJ_NITI];
    // Traženi minimumi
    double m[BROJ_NITI];
    // Vremena izvršavanja
    duration<double, milli> v[BROJ_NITI];

    // Pokretanje niti
    for (int i = 0; i < BROJ_NITI; i++) {
        t[i] = thread(f, A.begin() + A.size() * i / BROJ_NITI, A.begin() + A.size() * (i+1) / BROJ_NITI, ref(m[i]), ref(v[i]));
    }

    // Čekanje da se niti završe
    for (int i = 0; i < BROJ_NITI; i++) {
        t[i].join();
    }

    // Traženje minimuma dobijenih minimuma
    double min = m[0];
    for (int i = 1; i < BROJ_NITI; i++) {
        if (abs(m[i]) < abs(min)) {
            min = m[i];
        }
    }

    //Ispis rezultata
    cout << "Element najblizi nuli: " << min << endl;

    // Ispis vremena trajanja
    for (int i = 0; i < BROJ_NITI; i++) {
        duration<double, ratio<60>> mins= v[i];
        duration<double> secs= v[i];
        duration<double, milli> millis= v[i];
        cout << "Nit br. " << i << " se izvrsavala " << millis.count() << " milisekundi." << endl;
        cout << "Nit br. " << i << " se izvrsavala " << secs.count() << " sekundi." << endl;
        cout << "Nit br. " << i << " se izvrsavala " << mins.count() << " minuta." << endl;
        cout << endl;
    }
}

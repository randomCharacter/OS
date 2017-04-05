/*
Napraviti konkurentni program koji u funkciji visina pita korisnika koliko je visok.
Nakon toga korisnik unosi svoju visinu. Na kraju u funkciji se ispisuje uneta visina.

Sprečiti štetno preplitanje na terminalu korišćenjem klase mutex.

Kreirati 2 niti od date funkcije. Ispratiti ispis.
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// Onemogućava višestruki pristup resursu
mutex m;

// Funkcija koa učitava visinu, a zatim je ispisuje
void visina() {
    // Ukoliko je m već zaključan izvršavanje se neće nastaviti
    // sve dok se m ponovo ne otključa
    m.lock();
    int h;
    cout << "Koliko ste visoki?" << endl;
    cin >> h;
    cout << "Vasa visina je: " << h << endl;
    // Omogućava drugim nitima koje su pozvale m.lock() da
    // zaključcaju m i nastave sa izvršavanjem
    m.unlock();
}

int main() {
    // Kreiranje niti
    thread t1(visina);
    thread t2(visina);

    // Čekanje da se niti završe
    t1.join();
    t2.join();
    // Pomoću klase mutex se mogu onemogućiti višestruki pristupi
    // globalnim promenljivim i kritičnim delovima koda i na taj
    // način prividno steći mogućnost kontrole preključivanja

    return 0;
}

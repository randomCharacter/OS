/*
Napraviti konkurentni program koji u funkciji visina pita korisnika koliko je visok.
Nakon toga korisnik unosi svoju visinu. Na kraju u funkciji se ispisuje uneta visina.

Kreirati 2 niti od date funkcije. Ispratiti ispis.
*/

#include <iostream>
#include <thread>

using namespace std;

// Funkcija koa učitava visinu, a zatim je ispisuje
void visina() {
    int h;
    cout << "Koliko ste visoki?" << endl;
    cin >> h;
    cout << "Vasa visina je: " << h << endl;
}

int main() {
    // Kreiranje niti
    thread t1(visina);
    thread t2(visina);

    // Čekanje da se niti završe
    t1.join();
    t2.join();
    // Dolazi do štetnog preplitanja, zbog toga
    // što se ne može uticati na to kad će se desiti

    return 0;
}

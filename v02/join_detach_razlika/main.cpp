/*Napraviti konkurentni program koji stvara nit iz koje:
pita korisnika za visinu u centimetrima i
ispisuje: “Vasa visina je <uneta_visina> cm.”

Testirati program tako sto ce se nit prevesti iz stanja
joinable prvo operacijom join a nakon toga detach.
*/

#include <iostream>
#include <thread>

using namespace std;

void kod_niti() {
    int visina;
    cout << "Unesite visinu: ";
    cin >> visina;
    cout << "Visina je: " << visina << endl;
}

int main() {
    thread t(kod_niti);

    t.join();
    //t.detach();

    return 0;
}

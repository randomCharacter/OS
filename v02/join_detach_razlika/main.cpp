/*Napraviti konkurentni program koji stvara nit iz koje:
pita korisnika za visinu u centimetrima i
ispisuje: “Vasa visina je <uneta_visina> cm.”

Testirati program tako sto ce se nit prevesti iz stanja
joinable prvo operacijom join a nakon toga detach.
*/

#include <iostream>
#include <thread>

using namespace std;

void f() {
    int visina;
    cout << "Unesite visinu: ";
    cin >> visina;
    cout << "Vasa visina je " << visina << "cm." << endl;
}

int main() {
    // Kreiranje niti
    thread t(f);

    // join - čeka da se nit završi
    t.join();

    // detach - završava program, ali ostavlja nit da radi
    //t.detach();

    return 0;
}

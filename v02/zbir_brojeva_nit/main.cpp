/* Napraviti program koji kreira jednu nit kojoj se prosledjuju
dva cela broja a i b. U okviru niti sabrati brojeve i ispisati
njihov zbir*/

#include <iostream>
#include <thread>

using namespace std;

// Funkcija koja ispisuje zbir 2 broja
void f(const int a, const int b) {
    cout << a << "+" << b << "=" << a + b << endl;
}

int main() {
    // Kreiranje niti
    thread t(f, 5, 3);

    // Čekanje da se nit završi
    t.join();

    return 0;
}

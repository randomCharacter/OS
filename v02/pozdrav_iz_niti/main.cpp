/* Napraviti program koji kreira jednu nit i
u okviru niti ispisuje proizvoljnu recenicu*/

#include <iostream>
#include <thread>

using namespace std;

void pozdrav() {
    cout << "Pozdrav" << endl;
}

int main() {
    thread t(pozdrav);

    t.join();

    return 0;
}

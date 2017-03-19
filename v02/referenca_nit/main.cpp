/*
Definisati funkciju: void increment(int& a);
koja povećava (za jedan) vrednost argumenta.

Napraviti program koji:
a) poziva funkciju increment()
b) stvara nit od funkcije increment()
*/

#include <iostream>
#include <thread>

using namespace std;

void increment(int &a) {
    a++;
}

int main() {
    int a = 0;
    int b = 0;

    increment(a);

    thread t(increment, ref(b)); // ref() - operator reference
    t.join();

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    return 0;
}

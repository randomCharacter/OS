/*
Napraviti program koji prihvata proizvoljan broj linija
sa standardnog ulaza i zatim ih ispisuje u obrnutom
redosledu (poslednja prihvacena linija se ispisuje
prva).
*/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<string> v;
    string s;

    // Učitavanje stringova
    while (getline(cin, s)) {
        v.push_back(s); // Dodavanje novog stringa u vektor
    }

    // Obrnut ispis
    for (vector<string>::const_reverse_iterator it = v.rbegin(); it != v.rend(); it++) {
        cout << *it << endl;
    }


    return 0;
}

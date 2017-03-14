/*
Napraviti program koji prihvata proizvoljan broj linija
sa standardnog ulaza i zatim ih ispisuje u redosledu kojim su uneti.
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

    // Ispis
    for (vector<string>::const_iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << endl;
    }

    return 0;
}

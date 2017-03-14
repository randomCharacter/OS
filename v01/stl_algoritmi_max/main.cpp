/*
Uneti 5 celih brojeva sa standardnog ulaza.
Koriscenjem STL algoritma pronaci najveci uneti broj
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> v; // Vektor za čuvanje brojeva

    // Unos brojeva
    cout << "Unesite brojeve: ";
    for (int i = 0; i < 5; i++) {
        int t;
        cin >> t;
        v.push_back(t); // Dodavanje učitanog broja u vektor
    }

    // Pronalaženje maksimalnog elementa
    int m = *max_element(v.begin(), v.end());
    cout << "Najveci broj: " << m;

    return 0;
}

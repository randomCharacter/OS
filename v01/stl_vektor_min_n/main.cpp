/*Napraviti funkciju:

vector<int> min_n(const vector<int>& v,	int n);

Ova funkcija vraca vektor koji sadrzi n najmanjih
elemenata iz vektora v.
Podrazumeva se: v.size()>=n
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> min_n(const vector<int>& v,	int n) {
    vector<int> m(v); // Pomoćni vektor

    // Sortiranje pomoćnog vektora
    sort(m.begin(), m.end());

    vector<int> w(n);

    // Kopiranje najmanjih elemenata
    for (int i = 0; i < n; i++) {
        w[i] = m[i];
    }

    return w;
}

int main() {
    int n;

    // Učitavanje dužine
    cout << "Unesite duzinu vektora: ";
    cin >> n;

    // Kreiranje vektora
    vector<int> v(n);

    // Učitavanje vektora
    cout << "Unesite vektor v: ";
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    // Učitavanje broja minimuma
    int m;
    cout << "Unesite broj minimuma: ";
    cin >> m;

    // Poziv funkcije
    vector<int> w = min_n(v,m);

    // Ispis rezultata
    cout << "Rezultujuci vektor: ";
    for (int i = 0; i < m; i++) {
        cout << w[i] << " ";
    }
    cout << endl;

    return 0;
}

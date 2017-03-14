/* Napraviti funkciju:

vector<int> min(const vector<int>& a, const vector<int>& b);

Ova funkcija vraca vektor koji sadrzi minimume korespodentnih elemenata vetora a i b.
Npr: r[0]=min(a[0],b[0]), r[1]=...
Podrazumeva se: a.size()==b.size()
*/

#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

vector<int> min(const vector<int>& a, const vector<int>& b) {
    vector<int> c(a.size());
    for (int i = 0; i < a.size(); i++) {
        c[i] = min(a[i], b[i]);
    }
    return c;
}

int main() {
    int n;

    // Učitavanje
    cout << "Unesite duzinu vektora: ";
    cin >> n;

    // Kreiranje vektora dužine n
    vector<int> a(n);
    vector<int> b(n);

    // Učitavanje vektora a
    cout << "\nUnesite vektor a: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // Učitavanje vektora b
    cout << "\nUnesite vektor b: ";
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }

    // Pozivanje funkcije
    vector<int> c = min(a, b);

    // Ispis rezultata
    cout << "\nRezultat:";
    for (int i = 0; i < n; i++) {
        cout << c[i] << " ";
    }
    cout << endl;


    return 0;
}

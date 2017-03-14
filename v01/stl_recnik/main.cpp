/*
Napraviti jednostavan program za prevodenje reci sa engleskog na srpski jezik.
Nakon unosa reci sa standardnog ulaza, ispisuje  se prevod ukoliko
unesena rec postoji u recniku. Uneti 3 reci engleske reci i njihove prevode na srpski.
*/

#include <iostream>
#include <string>
#include <map>

using namespace std;

int main() {
    // Mapa - kljuc je engleska rec, a vrednost srpska
    map<string, string> recnik;

    recnik["first"] = "prvi";
    recnik["second"] = "drugi";
    recnik["third"] = "treci";

    // Uciravanje
    string rec;
    cout << "Unesite englesku rec: ";
    cin >> rec;

    // Prevod reči
    cout << "Prevod reci: ";
    cout << recnik[rec];

    return 0;
}

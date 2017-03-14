/* Napisati program koji omogucuje evidenciju fakultetskih ucionica.
Za svaku ucionicu potrebno je evidentirati naziv ucionice, kvadraturu i racunare
koji se nalaze u ucionici. Za svaki racunar evidentiraju se podaci o hard disku i
procesoru.

U okviru main funkcije, definisati jednu ucionicu koja sadrzi dva racunara.
Zatim je potrebno na konzolu ispisati hard disk i procesor svakog racunara u ucionici .
*/

#include <iostream>

#include "ucionica.h"
#include "racunar.h"

using namespace std;

int main() {

    Ucionica uc("MI2-3", 50);

    Racunar r1("Toshiba", "Intel");
    Racunar r2("Samsung", "AMD");

    uc.dodajRacunar(r1);
    uc.dodajRacunar(r2);

    // Ispis rezultata
    cout << "--------UCIONICA--------" << endl;
    cout << "NAZIV: " << uc.getNaziv() << endl;
    cout << "KVADRATURA: " << uc.getKvadratura() << endl;
    cout << "--------RACUNARI--------" << endl;
    for (int i = 0; i < 2; i++) {
        cout << "HDD:"  << uc.getRacunari()[i].getHdd() << " ";
        cout << "CPU:"  << uc.getRacunari()[i].getCpu() << endl;
    }

    return 0;
}

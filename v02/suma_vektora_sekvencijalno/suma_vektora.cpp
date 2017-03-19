#include "suma_vektora.h"

/*
pocetak - iterator na prvi element koji se uzima u obzir pri racunanju sume
kraj - iterator iza poslednjeg elementa koji se uzima u obzir pri racunanju sume
zbir - parameter kroz koji funkcija treba da vrati izracunatu sumu
*/
void f(ci pocetak, ci kraj, double& zbir) {
    zbir = 0;
    for (ci it = pocetak; it != kraj; it++) {
        zbir += *it;
    }
}

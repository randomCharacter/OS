#ifndef ANTONIM_H
#define ANTONIM_H

#include <map>
#include <string>

using namespace std;

// Funkcija koja će biti iskorišćena kao funkcija niti - u zadatom rečniku (parametar "recnik") pronaći suprotnu reč (antonim) od zadate reči "rec"
//
// recnik   - Rečnik koji sadrži parove reč - reč suprotnog značenja
// rec      - Reč za koju se traži reč suprotnog značenja
// rezultat - Reč suprotnog značenja (ako je pronađena, ako nije pronađena ovde treba da se upiše prazan string)
//
void nadjiAntonim(map<string, string> recnik, string rec, string& rezultat) {
    for (map<string, string>::const_iterator it = recnik.begin(); it != recnik.end(); it++) {
        if (it->first == rec) {
            rezultat = it->second;
            break;
        }

        if (it->second == rec) {
            rezultat = it->first;
            break;
        }
    }
}

#endif // ANTONIM_H

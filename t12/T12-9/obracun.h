#ifndef OBRACUN_H
#define OBRACUN_H

#include <string>
#include <map>
#include <list>

using namespace std;

void obracunajUkupno(list<string> kupljeno, map<string, double> cenovnik, double& ukupno) {
    ukupno = 0;

    for (list<string>::const_iterator it = kupljeno.begin(); it != kupljeno.end(); it++) {
        ukupno += cenovnik[*it];
    }
}

#endif // OBRACUN_H

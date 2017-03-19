#ifndef CENOVNIK_H
#define CENOVNIK_H

#include <string>
#include <map>

using namespace std;

void nadjiPovoljno(map<string, double> a, map<string, double> b, map<string, double> &jeftino) {
    for(map<string, double>::const_iterator it = a.begin(); it != a.end(); it++) {
        jeftino[it->first] = min(a[it->first], b[it->first]);
    }
}

#endif // CENOVNIK_H

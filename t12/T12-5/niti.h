#ifndef NITI_H
#define NITI_H

#include <thread>
#include <map>

using namespace std;

// Implementacija pokretanja dodatne niti koja treba da napravi novi rečnik (koristeći funkciju "napraviRecnik")
map<string, string> izracunaj(map<string, string> engSrp) {
    map<string, string> srpEng;

    thread t(napraviRecnik, engSrp, ref(srpEng));

    t.join();

    return srpEng;
}

#endif // NITI_H

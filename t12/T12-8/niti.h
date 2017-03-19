#ifndef NITI_H
#define NITI_H

#include <thread>

using namespace std;

map<string, double> izracunaj(map<string, double>& a, map<string, double>& b) {
   map<string, double> c;

   thread t(nadjiPovoljno, a, b, ref(c));

   t.join();

   return c;
}

#endif // NITI_H

#ifndef BROJAC_H_NCLUDED
#define BROJAC_H_INCLUDED

#include <iostream>
#include <mutex>

using namespace std;

class Brojac {
        int br;
        mutex m;
    public:
        void inc() {
            m.lock();
            br++;
            m.unlock();
        }

        void dec() {
            m.lock();
            br--;
            m.unlock();
        }

        friend ostream& operator<<(ostream& out, Brojac& brojac) {
            brojac.m.lock();
            out << brojac.br << endl;
            brojac.m.unlock();
            return out;
        }
};

#endif // BROJAC_H_INCLUDED

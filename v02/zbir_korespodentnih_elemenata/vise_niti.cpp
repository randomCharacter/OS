#include <iostream>
#include <vector>
#include <thread>
#include "vise_niti.h"
#include "suma_vektora.h"

using namespace std;

// v - vektor A cije elemente treba sumirati
// v - vektor B cije elemente treba sumirati
// povratna vrednost - suma korespondentnih elemenata vektora A i B, izracunata pozivanjem dve niti
vector<double> sumiraj(vector<double> a, vector<double> b) {

    vector<double> sum(a.size());

    ci a_begin = a.begin();
    ci a_middle = a.begin() + a.size() / 2;
    ci a_end = a.end();
    ci b_begin = b.begin();
    ci b_middle = b.begin() + b.size() / 2;
    vector<double>::iterator sum_begin = sum.begin();
    vector<double>::iterator sum_middle = sum.begin() + sum.size() / 2;

    thread t1(f, a_begin, a_middle, b_begin, sum_begin);
    thread t2(f, a_middle, a_end, b_middle, sum_middle);

    t1.join();
    t2.join();

    return sum;
}

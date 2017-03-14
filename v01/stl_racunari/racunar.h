#ifndef RACUNAR_H_INCLUDED
#define RACUNAR_H_INCLUDED

#include <string>

using namespace std;

class Racunar {
    private:
        string hdd;
        string cpu;
    public:
        Racunar(string hdd, string cpu);

        string getHdd();
        string getCpu();
};

#endif // RACUNAR_H_INCLUDED

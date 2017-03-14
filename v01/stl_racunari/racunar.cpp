#include "racunar.h"

Racunar::Racunar(string hdd, string cpu) : hdd(hdd), cpu(cpu) {}

string Racunar::getCpu() {
    return cpu;
}

string Racunar::getHdd() {
    return hdd;
}

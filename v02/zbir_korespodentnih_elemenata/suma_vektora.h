#ifndef SUMA_VEKTORA_H_INCLUDED
#define SUMA_VEKTORA_H_INCLUDED

#include <vector>

using namespace std;

typedef vector<double>::const_iterator ci;
void f(ci a_begin, ci a_end, ci b_begin, vector<double>::iterator sum_begin);

#endif // SUMA_VEKTORA_H_INCLUDED

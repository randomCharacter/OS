#include "suma_vektora.h"

/*
Funkcija niti. Funkcija preuzima sledece parametre:
1) ci a_begin -> iterator na pocetak dela vektora a
2) ci a_end   -> iterator na kraj dela vektora a (dovoljan je samo jedan kraj)
3) ci b_begin -> iterator na pocetak dela vektora b
4) vector<double>::iterator sum_begin  -> iterator na pocetak dela vektora suma
*/
void f(ci a_begin, ci a_end, ci b_begin, vector<double>::iterator sum_begin) {
    ci it_a, it_b = b_begin;
    for (ci it_a = a_begin; it_a != a_end; it_a++, it_b++, sum_begin++) {
        *sum_begin = *it_a + *it_b;
    }
}

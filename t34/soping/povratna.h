#ifndef POVRATNA_H_INCLUDED
#define POVRATNA_H_INCLUDED

using namespace std;
using namespace chrono;

struct povratna_vrednost {
	bool kupio;
	duration<double, milli> cekao_na_kabinu;
};

#endif // POVRATNA_H_INCLUDED

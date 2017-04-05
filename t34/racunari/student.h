#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED

#include <iostream>

using namespace std;

class Student {
public:
    // rbr         - Redni broj studenta
    void ceka(int rbr) {
        cout << "Student " << rbr << " ceka jer nema slobodnih racunara. " << endl;
    }

    // rbr         - Redni broj studenta
    // id_racunara - Redni broj racunara kojeg student oslobadja (prethodno je zauzeo taj racunar)
    void zauzeo(int rbr, int id_racunara) {
        cout << "Student " << rbr << " seo za racunar " << id_racunara << "." << endl;
    }

    // rbr         - Redni broj studenta
    // id_racunara - Redni broj racunara kojeg student oslobadja (prethodno je zauzeo taj racunar)
    void oslobodio(int rbr, int id_racunara) {
        cout << "Student " << rbr << " zavrsio rad na racunaru " << id_racunara << " i napusta ucionicu." << endl;
    }
};

#endif // STUDENT_H_INCLUDED

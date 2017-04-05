#ifndef RC_H_INCLUDED
#define RC_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "student.h"

using namespace std;

#define MAX 50 //maksimalni broj racunara u ucionici

class RC {
private:
    Student& student;
    mutex m;
    condition_variable cv;
    int slobodan[MAX];
    int racunara;
public:
	RC(Student& st, int br) : student(st) {
	    racunara = MAX;
        for (int i = 0; i < MAX; i++) {
            // Znači da je računar slobodan
            slobodan[i] = -1;
        }
    }

    // Metoda koju poziva nit koja simulira studenta kako bi student zauzeo mesto za racunarom.
    // Ova metoda je blokirajuca - ako nema slobodnih mesta, ona ce cekati dok se neko ne oslobodi.
    //
    // rbr - Redni broj studenta
    //
    // Potrebno je pozvati metodu student.ceka kada su racunari zauzeti i student mora da ceka.
    // Potrebno je pozvati metodu student.zauzeo kada student zauzme racunar.
    int zauzmi(int rbr) {
        unique_lock<mutex> l(m);
        // Ukoliko su svi zauzeti
        while (racunara == 0) {
            student.ceka(rbr);
            cv.wait(l);
        }
        // Kada se neki oslobodi
        int brRac = MAX - racunara-- + 1;
        student.zauzeo(rbr, brRac);
        return brRac;
    }

    // Metoda koju poziva nit koja simulira studenta kako bi oslobodio racunar koji je prethodno zauzeo.
    //
    // rbr         - Redni broj studenta
    // id_racunara - Redni broj racunara kojeg student oslobadja (prethodno je zauzeo taj racunar)
    //
    // Potrebno je pozvati metodu student.oslobodio kada student oslobodi racunar.
    void oslobodi(int rbr, int id_racunara) {
        unique_lock<mutex> l(m);
        student.oslobodio(rbr, id_racunara);
        cv.notify_one();
    }
};

#endif // RC_H_INCLUDED

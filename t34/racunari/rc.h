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
    bool slobodan[MAX];
    int racunara, ukRacunara;

public:
	RC(Student& st, int br) : student(st) {
	    ukRacunara = br;
	    racunara = ukRacunara;
        for (int i = 0; i < ukRacunara; i++) {
            // Znači da je računar slobodan
            slobodan[i] = true;
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
        // Broj računara za koji student seda
        int brRac;
        for (int i = 0; i < ukRacunara; i++) {
            if (slobodan[i]) {
                brRac = i;
                break;
            }
        }
        racunara--;
        slobodan[brRac] = false;
        // Kada se neki oslobodi
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
        // Oslobađa računar i obaveštava druge niti o tome
        student.oslobodio(rbr, id_racunara);
        slobodan[id_racunara] = true;
        racunara++;
        cv.notify_one();
    }
};

#endif // RC_H_INCLUDED

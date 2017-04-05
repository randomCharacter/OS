#ifndef AMBULANTA_H_INCLUDED
#define AMBULANTA_H_INCLUDED

#include <mutex>
#include <condition_variable>

#include "student.h"

using namespace std;

class Studentska_ambulanta {
private:
    Student& student;
    mutex m;
    condition_variable cvm, cvz;
    int mMesta, zMesta;

public:
    Studentska_ambulanta(Student& st, int muskih, int zenskih) : student(st) {
        mMesta = muskih;
        zMesta = zenskih;
    }

    // Metoda koju poziva nit koja simulira studenta u ambulanti kada student pokusava da udje u nju.
    // Ukoliko je ambulanta zauzeta, izvrsenje ove metode ce cekati da se ona oslobodi.
    //
    // rbr - Redni broj studenta
    // p   - Pol studenta
    //
    // Potrebno je pozvati metodu student.ceka kada student ne moze da stupi u ambulantu.
    // Potrebno je pozvati metodu student.ulazi kada student udje u ambulantu.
    void udji(int rbr, Pol p) {
        unique_lock<mutex> l(m);
        if (p == MUSKI) {
            // Ukoliko nema slobodnih mesta
            while (mMesta == 0) {
                student.ceka(rbr, p);
                // Čeka da se mesto oslobodi
                cvm.wait(l);
            }
            // Kad se mesto oslobodi
            mMesta--;
            student.ulazi(rbr, p);
        } else {
            // Ukoliko nema slobodnih mesta
            while (zMesta == 0) {
                student.ceka(rbr, p);
                // Čeka da se mesto oslobodi
                cvz.wait(l);
            }
            // Kad se mesto oslobodi
            zMesta--;
            student.ulazi(rbr, p);
        }
    }

    // Metoda koju poziva nit koja simulira studenta u ambulanti nakon sto je student pregledan i izlazi iz nje.
    //
    // rbr - Redni broj studenta
    // p   - Pol studenta
    void izadji(int rbr, Pol p) {
        unique_lock<mutex> l(m);
        if (p == MUSKI) {
            // Mesto se oslobađa
            mMesta++;
            // Obaveštava druge niti da se broj muških mesta promenio
            cvm.notify_one();
        } else {
            // Mesto se oslobađa
            zMesta++;
            // Obaveštava druge niti da se broj ženskih mesta promenio
            cvz.notify_one();
        }
    }
};

#endif // AMBULANTA_H_INCLUDED

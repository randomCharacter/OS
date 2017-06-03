#include "cv_tag.h"

cv_tag::cv_tag() {
}

// Destruktor mape, brišu se sve vrednosti jer su dinamički genreisane (vidi metodu wait):
cv_tag::~cv_tag() {
    for (auto i = waiting.begin(); i!=waiting.end(); ++i)
        delete (*i).second;
}

// Čekanje na CV sa privescima, prosleđuje se lock po referenci i privezak:
void cv_tag::wait(unique_lock<mutex>& l, size_t tag) {
    // Ubacivanje novog para ključ-vrednost u mapu (ključ je privezak, vrednost je cv_data)
    waiting.insert(pair<size_t,cv_data*>{tag,new cv_data});
    while (!waiting[tag]->fulfilled)    // Dok god se nije ispunio uslov čekanja na cv_data sa tim priveskom, nit će čekati.
        waiting[tag]->c.wait(l);

    delete waiting[tag];				// Kada se ispunio uslov, briše se cv_data iz memorije kao i pair struktura iz mape.
    waiting.erase(tag);
}

// Metoda notifikacije na CV sa privescima, prosleđuje se privezak koji se obaveštava:
void cv_tag::notify(size_t tag) {
    if (waiting.find(tag) != waiting.end()) {   // Prolazi se kroz mapu i traži se ključ (privezak)
        waiting[tag]->fulfilled = true;			// Kada se nađe privezak, polje fulfilled cv_data strukture (vrednosti) se postavlja na true.
        waiting[tag]->c.notify_one();			// Obaveštava se nit koja čeka na datoj cv_data.
    }
}

// Notifikacija svih na CV sa privescima, prolazi se kroz mapu svih i obaveštavaju se jedan po jedan:
void cv_tag::notify_all() {
	for (auto i = waiting.begin(); i != waiting.end(); ++i) {
            (*i).second->fulfilled = true;
            (*i).second->c.notify_one();
    }
}


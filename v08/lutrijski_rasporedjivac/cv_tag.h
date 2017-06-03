#ifndef CV_TAG_H_INCLUDED
#define CV_TAG_H_INCLUDED

#include <thread>
#include <map>
#include <condition_variable>
#include <mutex>

using namespace std;
using namespace chrono;

// Klasa cv_tag simulira condition varijablu sa privescima gde je moguće obaveštavati niti po izabranom privesku
class cv_tag {
	private:
		struct cv_data {		// Stvaraju se zasebne CV za svako novo čekanje. Ove CV se nalaze u cv_data strukturi.
			condition_variable c;
			bool fulfilled;
			cv_data(): fulfilled(false) {}
		};
		map<size_t, cv_data*> waiting;	// Mapa prethodno opisanih struktura kako bi se moglo stavljati i brisati iz mape sa proizvoljnih mesta
	public:
			cv_tag();
   			~cv_tag();
			void wait(unique_lock<mutex>& ,size_t);
			void notify(size_t tag);
			void notify_all();
};

#endif // CV_TAG_H_INCLUDED

#include "cv_hrono.h"

void cv_hrono::wait(unique_lock<mutex>& lock) {		//metoda cekanja na cv_hrono, mora da se prosledi referenca (ne moze kopija) na lock
      cv_pair* pair = new cv_pair;			//stvara se nova struktura cv_pair kada se ulazi u wait
      ccv.push_back(pair);					//ova struktura se stavlja na kraj reda ccv (FIFO)
      while(!pair->fulfilled)			//na ovoj strukturi se ceka dok se bool fulfilled ne postavi na true i tek tada se izlazi iz cekanja
         pair->cv.wait(lock);
      while(ccv.front()!=pair) {		//semanticka provera - da li je zaista cv koja je izasla iz cekanja ona prva is reda (front), ukoliko
         lock.unlock();					//slucajno iz nekog razloga nije prepusta se procesor drugoj niti (yield) i nakon toga se ponovo 
         this_thread::yield();			//proverava
         lock.lock();
      }
      ccv.pop_front();					//skidanje strukture sa vrha reda (FIFO), i brisanje strukture iz memorije
      delete pair;
}      

cv_status cv_hrono::wait_for(unique_lock<mutex>& lock, int sekundi) {		//metoda ogranicenog cekanja na cv_hrono 
      cv_pair* pair = new cv_pair;								//funkcionise veoma slicno kao metoda wait, sa tim sto ako se desi timeout
      ccv.push_back(pair);										//prekida se cekanje
	  cv_status status;
      while(!pair->fulfilled) {
        status = pair->cv.wait_for(lock, seconds(sekundi));
        if (status == cv_status::timeout)
            break;
	  }
      ccv.pop_front();    
      delete pair;   
	  return status;
}

bool cv_hrono::notify_one() {						//metoda notifikacije na cv_hrono
	  if(ccv.empty()) 								//ukoliko nema nikoga u cekanju vraca se false
		 return false;		
	  ccv.front()->fulfilled = true;				//ukoliko ima nekog, uzima se prvi iz reda cekanja (FIFO) i obavestava se
	  ccv.front()->cv.notify_one();
	  return true;
}

void cv_hrono::notify_all() {					//metoda notifikacije svih na cv_hrono
      for(auto i = ccv.begin(); i != ccv.end(); ++i) {	//prolazi se kroz citav red cekanja i obavestavaju se svi - jedan po jedan
         (*i)->fulfilled = true;
         (*i)->cv.notify_one();
      }
}




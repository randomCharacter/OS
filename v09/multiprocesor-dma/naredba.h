#ifndef NAREDBA_H_INCLUDED
#define NAREDBA_H_INCLUDED

// Konstruktori su napravljeni tako da omoguće jednostavno stvaranje naredbi.
// Čitanje: 1 parametar = adresa lokacije sa koje se čita vrednost
// Pisanje: 2 parametra = adresa lokacije u koju se upisuje vrednost,
//                        vrednost koja se upisuje
// DMA:     3 parametra = adresa početka bloka memorije koji se kopira,
//                        broj lokacija čiji se sadržaj kopira,
//                        adresa početka bloka memorije u koju se kopira
//
// (Vidi vektore I, II i III u funkciji main().)

struct Naredba {
    Magistrala::Stanje tip_transfera;
    int odakle;
    int koliko;
    int kome;
    char vrednost;
    Naredba(int adresa)
        : tip_transfera(Magistrala::MEM_CITAJ), odakle(adresa) {}
    Naredba(int adresa, char v)
        : tip_transfera(Magistrala::MEM_PISI), kome(adresa), vrednost(v) {}
    Naredba(int o, int kom, int kol)
       : tip_transfera(Magistrala::DMA), odakle(o), koliko(kol), kome(kom) {}
};

#endif // NAREDBA_H_INCLUDED

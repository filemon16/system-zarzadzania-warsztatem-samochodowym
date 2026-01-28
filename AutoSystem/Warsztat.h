#ifndef WARSZTAT_H
#define WARSZTAT_H

#include <vector>
#include <string>

class Osoba;
class Klient;
class Pracownik;
class Samochod;
class Zlecenie;
class Czesc;

class Warsztat {
public:
    Warsztat();
    void dodajKlienta(Klient* klient);
    void dodajPracownika(Pracownik* pracownik);
    void dodajZlecenie(Zlecenie* zlecenie);
    void dodajCzesc(Czesc* czesc);
    void wczytaj(const std::string& sciezka);
    void zapisz(const std::string& sciezka);
    void generujRaport(const std::string& sciezka);
    const std::vector<Klient*>& getKlienci() const { return m_klienci; }
    const std::vector<Pracownik*>& getPracownicy() const { return m_pracownicy; }
    const std::vector<Zlecenie*>& getZlecenia() const { return m_zlecenia; }
    const std::vector<Czesc*>& getCzesci() const { return m_czesci; }

private:
    std::vector<Klient*> m_klienci;
    std::vector<Pracownik*> m_pracownicy;
    std::vector<Zlecenie*> m_zlecenia;
    std::vector<Czesc*> m_czesci;
};

#endif

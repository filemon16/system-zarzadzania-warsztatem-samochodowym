#ifndef KLIENT_H
#define KLIENT_H

#include "Osoba.h"
#include <vector>
#include <string>

class Samochod;

class Klient : public Osoba {
public:
    Klient(const std::string& imie, const std::string& nazwisko);

    void przedstawSie() const override;

    void dodajSamochod(Samochod* samochod);
    const std::vector<Samochod*>& getPojazdy() const { return m_pojazdy; }

private:
    std::vector<Samochod*> m_pojazdy;
};

#endif

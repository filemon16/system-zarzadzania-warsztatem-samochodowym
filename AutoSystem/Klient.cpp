#include "Klient.h"
#include "Samochod.h"
#include <iostream>

Klient::Klient(const std::string& imie, const std::string& nazwisko)
    : Osoba(imie, nazwisko) {}

void Klient::przedstawSie() const {
    std::cout << "Jestem klient " << m_imie << " " << m_nazwisko << ".\n";
}

void Klient::dodajSamochod(Samochod* samochod) {
    if (samochod) {
        m_pojazdy.push_back(samochod);
    }
}

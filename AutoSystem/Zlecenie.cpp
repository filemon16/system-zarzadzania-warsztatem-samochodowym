// Zlecenie.cpp
#include "Zlecenie.h"
#include "Czesc.h"

Zlecenie::Zlecenie(Klient* klient, Samochod* pojazd, const std::string& opisUsterki)
    : m_klient(klient), m_pojazd(pojazd), m_mechanik(nullptr), m_status(NOWE), m_opisUsterki(opisUsterki), m_koszt(0.0) {}

Zlecenie::PostepNaprawy::PostepNaprawy(const std::string& data, const std::string& opis)
    : m_data(data), m_opis(opis) {}

void Zlecenie::dodajPostep(const std::string& data, const std::string& opis) {
    m_historia.emplace_back(data, opis);
}

void Zlecenie::dodajCzesc(Czesc* czesc, int ilosc) {
    if (czesc && ilosc > 0) {
        m_czesci.push_back({czesc, ilosc});
        obliczKoszt(); // Przelicz koszt po dodaniu części
    }
}

void Zlecenie::ustawMechanika(Pracownik* mechanik) {
    m_mechanik = mechanik;
}

void Zlecenie::ustawStatus(StatusZlecenia status) {
    m_status = status;
}

void Zlecenie::dodajCzynnosc(const std::string& czynnosc) {
    m_czynnosci.push_back(czynnosc);
}

void Zlecenie::obliczKoszt() {
    m_koszt = 0.0;
    for (const auto& p : m_czesci) {
        Czesc* c = p.first;
        int ilosc = p.second;
        m_koszt += c->getCena() * ilosc;
    }
}

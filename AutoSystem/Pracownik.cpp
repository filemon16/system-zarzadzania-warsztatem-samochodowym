#include "Pracownik.h"
#include <iostream>

Pracownik::Pracownik(const std::string& imie, const std::string& nazwisko, const std::string& stanowisko, const std::string& haslo)
    : Osoba(imie, nazwisko), m_stanowisko(stanowisko), m_haslo(haslo) {}

void Pracownik::przedstawSie() const {
    std::cout << "Jestem " << m_imie << " " << m_nazwisko << ", pracuję jako " << m_stanowisko << ".\n";
}

Mechanik::Mechanik(const std::string& imie, const std::string& nazwisko, const std::string& haslo)
    : Pracownik(imie, nazwisko, "Mechanik", haslo) {}

void Mechanik::przedstawSie() const {
    std::cout << "Jestem mechanik " << m_imie << " " << m_nazwisko << ".\n";
}

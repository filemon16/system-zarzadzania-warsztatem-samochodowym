#ifndef PRACOWNIK_H
#define PRACOWNIK_H

#include "Osoba.h"
#include <string>

class Pracownik : public Osoba {
public:
    Pracownik(const std::string& imie, const std::string& nazwisko, const std::string& stanowisko, const std::string& haslo);
    void przedstawSie() const override;
    std::string getStanowisko() const { return m_stanowisko; }
private:
    std::string m_stanowisko;
    std::string m_haslo;
};

class Mechanik : public Pracownik {
public:
    Mechanik(const std::string& imie, const std::string& nazwisko, const std::string& haslo);
    void przedstawSie() const override;
};

#endif

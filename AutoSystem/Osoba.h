#ifndef OSOBA_H
#define OSOBA_H

#include <string>

class Osoba {
public:
    Osoba(const std::string& imie, const std::string& nazwisko);
    virtual ~Osoba() = default;
    virtual void przedstawSie() const = 0;
    std::string getImie() const { return m_imie; }
    std::string getNazwisko() const { return m_nazwisko; }
protected:
    std::string m_imie;
    std::string m_nazwisko;
};

#endif

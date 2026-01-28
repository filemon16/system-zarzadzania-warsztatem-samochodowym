#ifndef CZESC_H
#define CZESC_H

#include <string>

class Czesc {
public:
    Czesc(const std::string& nazwa, double cena, int ilosc);

    std::string getNazwa() const { return m_nazwa; }
    double getCena() const { return m_cena; }
    int getIlosc() const { return m_ilosc; }


    bool czyBrakujaca(int ilosc_potrzebna) const { return m_ilosc < ilosc_potrzebna; }
    void zmniejszIlosc(int ilosc) { if (m_ilosc >= ilosc) m_ilosc -= ilosc; }

private:
    std::string m_nazwa;
    double m_cena;
    int m_ilosc;
};

#endif

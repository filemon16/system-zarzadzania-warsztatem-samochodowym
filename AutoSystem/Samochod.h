#ifndef SAMOCHOD_H
#define SAMOCHOD_H

#include <string>

class Klient;

class Samochod {
public:
    Samochod(const std::string& marka, const std::string& model, int rok, Klient* wlasciciel);

    std::string getMarka() const { return m_marka; }
    std::string getModel() const { return m_model; }
    int getRok() const { return m_rok; }
    Klient* getWlasciciel() const { return m_wlasciciel; }

private:
    std::string m_marka;
    std::string m_model;
    int m_rok;
    Klient* m_wlasciciel;
};

#endif

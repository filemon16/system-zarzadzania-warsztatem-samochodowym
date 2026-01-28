#include "Samochod.h"

Samochod::Samochod(const std::string& marka, const std::string& model, int rok, Klient* wlasciciel)
    : m_marka(marka), m_model(model), m_rok(rok), m_wlasciciel(wlasciciel) {}

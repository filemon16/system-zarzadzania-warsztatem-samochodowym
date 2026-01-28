#ifndef ZLECENIE_H
#define ZLECENIE_H

#include <string>
#include <vector>
#include <utility>

class Klient;
class Samochod;
class Czesc;
class Pracownik;

class Zlecenie {
public:
    enum StatusZlecenia {
        NOWE,
        W_TRAKCIE,
        ZAKONCZONE
    };

    class PostepNaprawy {
    public:
        PostepNaprawy(const std::string& data, const std::string& opis);
        std::string getData() const { return m_data; }
        std::string getOpis() const { return m_opis; }
    private:
        std::string m_data;
        std::string m_opis;
    };

    Zlecenie(Klient* klient, Samochod* pojazd, const std::string& opisUsterki);
    void dodajPostep(const std::string& data, const std::string& opis);
    void dodajCzesc(Czesc* czesc, int ilosc);
    void ustawMechanika(Pracownik* mechanik);
    void ustawStatus(StatusZlecenia status);
    void dodajCzynnosc(const std::string& czynnosc);
    void obliczKoszt();

    Klient* getKlient() const { return m_klient; }
    Samochod* getPojazd() const { return m_pojazd; }
    Pracownik* getMechanik() const { return m_mechanik; }
    StatusZlecenia getStatus() const { return m_status; }
    const std::string& getOpisUsterki() const { return m_opisUsterki; }
    const std::vector<PostepNaprawy>& getHistoria() const { return m_historia; }
    const std::vector<std::pair<Czesc*, int>>& getCzesci() const { return m_czesci; }
    const std::vector<std::string>& getCzynnosci() const { return m_czynnosci; }
    double getKoszt() const { return m_koszt; }

private:
    Klient* m_klient;
    Samochod* m_pojazd;
    Pracownik* m_mechanik;
    StatusZlecenia m_status;
    std::string m_opisUsterki;
    std::vector<PostepNaprawy> m_historia;
    std::vector<std::pair<Czesc*, int>> m_czesci;
    std::vector<std::string> m_czynnosci;
    double m_koszt = 0.0;
};

#endif

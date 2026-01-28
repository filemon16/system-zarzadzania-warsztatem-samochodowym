#include "Warsztat.h"
#include "Klient.h"
#include "Zlecenie.h"
#include "Czesc.h"
#include "Samochod.h"
#include "Pracownik.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <sstream>

Warsztat::Warsztat() = default;

void Warsztat::dodajKlienta(Klient* klient) {
    if (klient) {
        m_klienci.push_back(klient);
    }
}

void Warsztat::dodajPracownika(Pracownik* pracownik) {
    if (pracownik) {
        m_pracownicy.push_back(pracownik);
    }
}

void Warsztat::dodajZlecenie(Zlecenie* zlecenie) {
    if (zlecenie) {
        m_zlecenia.push_back(zlecenie);
    }
}

void Warsztat::dodajCzesc(Czesc* czesc) {
    if (czesc) {
        m_czesci.push_back(czesc);
    }
}

void Warsztat::wczytaj(const std::string& sciezka) {
    QFile file(QString::fromStdString(sciezka));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString currentSection;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (line.startsWith("===")) {
            currentSection = line;
            continue;
        }

        if (currentSection.contains("KLIENCI")) {
            if (line.startsWith("Klient: ")) {
                QString clientLine = line.mid(8);
                QStringList parts = clientLine.split(" ");
                if (parts.size() >= 2) {
                    std::string imie = parts[0].toStdString();
                    std::string nazwisko = parts[1].toStdString();
                    Klient* klient = new Klient(imie, nazwisko);
                    m_klienci.push_back(klient);
                }
            }
        }
        else if (currentSection.contains("PRACOWNICY")) {
            if (line.startsWith("Pracownik: ")) {
                QString workerLine = line.mid(11);
                QStringList parts = workerLine.split(",");
                if (parts.size() >= 2) {
                    QString namePart = parts[0].trimmed();
                    QStringList nameParts = namePart.split(" ");
                    if (nameParts.size() >= 2) {
                        std::string imie = nameParts[0].toStdString();
                        std::string nazwisko = nameParts[1].toStdString();

                        QString rolePart = parts[1].trimmed();
                        if (rolePart.startsWith("Stanowisko: ")) {
                            std::string stanowisko = rolePart.mid(12).toStdString();
                            Pracownik* pracownik = new Pracownik(imie, nazwisko, stanowisko, "default123");
                            m_pracownicy.push_back(pracownik);
                        }
                    }
                }
            }
        }
        else if (currentSection.contains("ZLECENIA")) {
            if (line.startsWith("Zlecenie: ")) {
                QStringList parts = line.split(", ");
                std::string imieKlienta, nazwiskoKlienta;
                std::string markaPojazdu, modelPojazdu;
                int rokPojazdu = 0;
                std::string imieMechanika, nazwiskoMechanika;
                std::string statusStr;
                std::string opisUsterki;

                for (const auto& part : parts) {
                    if (part.startsWith("Klient: ")) {
                        QString klientInfo = part.mid(8);
                        QStringList nameParts = klientInfo.split(" ");
                        if (nameParts.size() >= 2) {
                            imieKlienta = nameParts[0].toStdString();
                            nazwiskoKlienta = nameParts[1].toStdString();
                        }
                    } else if (part.startsWith("Pojazd: ")) {
                        QString pojazdInfo = part.mid(8);
                        QStringList pojazdParts = pojazdInfo.split(" ");
                        if (pojazdParts.size() >= 3) {
                            markaPojazdu = pojazdParts[0].toStdString();
                            modelPojazdu = pojazdParts[1].toStdString();
                            rokPojazdu = pojazdParts[2].toInt();
                        }
                    } else if (part.startsWith("Mechanik: ")) {
                        QString mechanikInfo = part.mid(10);
                        QStringList nameParts = mechanikInfo.split(" ");
                        if (nameParts.size() >= 2) {
                            imieMechanika = nameParts[0].toStdString();
                            nazwiskoMechanika = nameParts[1].toStdString();
                        }
                    } else if (part.startsWith("Status: ")) {
                        statusStr = part.mid(8).toStdString();
                    } else if (part.startsWith("Usterka: ")) {
                        opisUsterki = part.mid(9).toStdString();
                    }
                }

                Klient* klient = nullptr;
                for (auto* k : m_klienci) {
                    if (k->getImie() == imieKlienta && k->getNazwisko() == nazwiskoKlienta) {
                        klient = k;
                        break;
                    }
                }

                Samochod* pojazd = nullptr;
                if (klient) {
                    pojazd = new Samochod(markaPojazdu, modelPojazdu, rokPojazdu, klient);
                }

                if (klient && pojazd) {
                    Zlecenie* zlecenie = new Zlecenie(klient, pojazd, opisUsterki);

                    // Ustaw status
                    if (statusStr == "NOWE") {
                        zlecenie->ustawStatus(Zlecenie::NOWE);
                    } else if (statusStr == "W TRAKCIE") {
                        zlecenie->ustawStatus(Zlecenie::W_TRAKCIE);
                    } else if (statusStr == "ZAKONCZONE") {
                        zlecenie->ustawStatus(Zlecenie::ZAKONCZONE);
                    }

                    for (auto* p : m_pracownicy) {
                        if (p->getImie() == imieMechanika && p->getNazwisko() == nazwiskoMechanika) {
                            zlecenie->ustawMechanika(p);
                            break;
                        }
                    }

                    m_zlecenia.push_back(zlecenie);
                }
            }
        }
        else if (currentSection.contains("CZĘŚCI")) {
            if (line.startsWith("Czesc: ")) {
                QString partLine = line.mid(7); // "Czesc: " length
                QStringList parts = partLine.split(",");
                if (parts.size() >= 3) {
                    QString nazwa = parts[0].trimmed();
                    QString cenaStr = parts[1].trimmed().split(":")[1].trimmed();
                    QString iloscStr = parts[2].trimmed().split(":")[1].trimmed();

                    std::string nazwaStr = nazwa.toStdString();
                    double cena = cenaStr.toDouble();
                    int ilosc = iloscStr.toInt();

                    Czesc* czesc = new Czesc(nazwaStr, cena, ilosc);
                    m_czesci.push_back(czesc);
                }
            }
        }
    }
    file.close();
}

void Warsztat::zapisz(const std::string& sciezka) {
    QFile file(QString::fromStdString(sciezka));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "=== KLIENCI ===\n";
    for (const auto& k : m_klienci) {
        out << "Klient: " << QString::fromStdString(k->getImie()) << " " << QString::fromStdString(k->getNazwisko()) << "\n";
    }
    out << "\n=== PRACOWNICY ===\n";
    for (const auto& p : m_pracownicy) {
        out << "Pracownik: " << QString::fromStdString(p->getImie()) << " " << QString::fromStdString(p->getNazwisko()) << ", Stanowisko: " << QString::fromStdString(p->getStanowisko()) << "\n";
    }
    out << "\n=== ZLECENIA ===\n";
    for (const auto& z : m_zlecenia) {
        QString status = "Nieznany";
        switch (z->getStatus()) {
        case Zlecenie::NOWE: status = "NOWE"; break;
        case Zlecenie::W_TRAKCIE: status = "W TRAKCIE"; break;
        case Zlecenie::ZAKONCZONE: status = "ZAKONCZONE"; break;
        }
        out << "Zlecenie: Klient: " << QString::fromStdString(z->getKlient()->getImie()) << " " << QString::fromStdString(z->getKlient()->getNazwisko())
            << ", Pojazd: " << QString::fromStdString(z->getPojazd()->getMarka()) << " " << QString::fromStdString(z->getPojazd()->getModel()) << " " << z->getPojazd()->getRok()
            << ", Mechanik: ";
        if (z->getMechanik()) {
            out << QString::fromStdString(z->getMechanik()->getImie()) << " " << QString::fromStdString(z->getMechanik()->getNazwisko());
        } else {
            out << "Brak";
        }
        out << ", Status: " << status
            << ", Usterka: " << QString::fromStdString(z->getOpisUsterki()) << "\n";
        out << "  - Czynności: ";
        for (const auto& c : z->getCzynnosci()) {
            out << QString::fromStdString(c) << "; ";
        }
        out << "\n";
        out << "  - Czesci: ";
        for (const auto& cp : z->getCzesci()) {
            Czesc* czesc = cp.first;
            int ilosc = cp.second;
            out << QString::fromStdString(czesc->getNazwa()) << "(" << ilosc << ") ";
        }
        out << "\n";
        if (z->getStatus() == Zlecenie::ZAKONCZONE) {
            out << "  - Koszt: " << z->getKoszt() << " zł\n";
        }
    }
    out << "\n=== CZĘŚCI ===\n";
    for (const auto& c : m_czesci) {
        out << "Czesc: " << QString::fromStdString(c->getNazwa()) << ", Cena: " << c->getCena() << ", Ilosc: " << c->getIlosc() << "\n";
    }
    file.close();
}

void Warsztat::generujRaport(const std::string& sciezka) {
    QFile file(QString::fromStdString(sciezka));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "Raport z systemu zarzadzania warsztatem\n";
    out << "Data: " << QDateTime::currentDateTime().toString() << "\n";
    out << "Liczba klientow: " << m_klienci.size() << "\n";
    out << "Liczba pracownikow: " << m_pracownicy.size() << "\n";
    out << "Liczba zlecen: " << m_zlecenia.size() << "\n";
    out << "Liczba czesci: " << m_czesci.size() << "\n";

    out << "\nSzczegoly:\n";
    out << "\nZlecenia:\n";
    for (const auto& z : m_zlecenia) {
        QString status = "Nieznany";
        switch (z->getStatus()) {
        case Zlecenie::NOWE: status = "NOWE"; break;
        case Zlecenie::W_TRAKCIE: status = "W TRAKCIE"; break;
        case Zlecenie::ZAKONCZONE: status = "ZAKONCZONE"; break;
        }
        out << "- Klient: " << QString::fromStdString(z->getKlient()->getImie()) << " " << QString::fromStdString(z->getKlient()->getNazwisko())
            << ", Pojazd: " << QString::fromStdString(z->getPojazd()->getMarka()) << " " << QString::fromStdString(z->getPojazd()->getModel()) << " " << z->getPojazd()->getRok()
            << ", Mechanik: ";
        if (z->getMechanik()) {
            out << QString::fromStdString(z->getMechanik()->getImie()) << " " << QString::fromStdString(z->getMechanik()->getNazwisko());
        } else {
            out << "Brak";
        }
        out << ", Status: " << status << "\n";

        out << "  - Czesci: ";
        for (const auto& cp : z->getCzesci()) {
            Czesc* czesc = cp.first;
            int ilosc = cp.second;
            out << QString::fromStdString(czesc->getNazwa()) << "(" << ilosc << ") ";
        }
        out << "\n";

        if (z->getStatus() == Zlecenie::ZAKONCZONE) {
            out << "  - Koszt: " << z->getKoszt() << " zł\n";
        }
    }
    file.close();
}

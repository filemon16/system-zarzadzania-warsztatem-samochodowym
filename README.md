Zespół: 6
Grupa lk:2
Osoby:
Michał Kozielec
Maciej Lis



System Zarządzania Warsztatem Samochodowym


Opis projektu

Prosty system zarządzania warsztatem samochodowym napisany w C++ z wykorzystaniem biblioteki Qt. Aplikacja umożliwia zarządzanie klientami, mechanikami, zleceniami, samochodami i częściami.


Funkcjonalności

    Klienci: Dodawanie i zarządzanie danymi klientów
    Mechanicy: Rejestracja pracowników warsztatu
    Zlecenia: Tworzenie i śledzenie zleceń napraw
    Samochody: Przypisanie pojazdów do klientów
    Części: Zarządzanie magazynem części
    Dodawanie części do zleceń: Możliwość przypisania części do konkretnego zlecenia
    Statusy zleceń: NOWE, W TRAKCIE, ZAKONCZONE
    Automatyczne obliczanie kosztów: Sumowanie kosztów części po zakończeniu zlecenia
    Symulacja powiadomień SMS: Po zakończeniu zlecenia
    Import/eksport danych: Zapis i odczyt danych z plików tekstowych
    Generowanie raportów: Podsumowanie danych w formacie tekstowym

Technologie

    C++
    Qt Framework
    Qt Creator IDE

Uruchamianie

    Skompiluj projekt w Qt Creator
    Uruchom aplikację
    Użyj interfejsu graficznego do zarządzania danymi

Struktura danych

    Klienci posiadają imię i nazwisko
    Mechanicy mają dodatkowo hasło i stanowisko
    Zlecenia zawierają opis usterki, przypisanego klienta, pojazd i mechanika
    Części posiadają nazwę, cenę i ilość w magazynie
    Status zlecenia wpływa na wyświetlanie kosztów i dostępne operacje

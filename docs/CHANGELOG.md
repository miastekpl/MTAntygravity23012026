# CHANGELOG
## Historia Zmian - System Malowania Pasów Drogowych

---

## [1.3.0] - 2026-01-25

### 🚀 Optymalizacje Produkcyjne i Bezpieczeństwo

#### Priorytet 1 - Krytyczne
- ✅ **Watchdog Timer (WDT)** - Zabezpieczenie przed zawieszeniem (5s timeout)
- ✅ **Bezpieczny ISR** - Szybki odczyt rejestrów i sekcje krytyczne (Atomic)
- ✅ **NVS (Preferences)** - Ochrona pamięci flash (Wear Leveling) zamiast EEPROM
- ✅ **Non-blocking Debounce** - Całkowite usunięcie `delay()` z pętli głównej

#### Priorytet 2 - Ważne
- ✅ **Buforowanie GPIO** - Eliminacja zbędnych operacji zapisu na przekaźnikach
- ✅ **Robust Math** - Zabezpieczenie przed dzieleniem przez zero w algorytmach powierzchni
- ✅ **Debug Macro** - Optymalizacja komunikatów szeregowych (kompilacja warunkowa)

#### Funkcje (z v1.1.0)
- ✅ **Nowe mapowanie pistoletów** (P1-P6)
- ✅ **Dedykowany selektor** (GPIO 19)
- ✅ **Start Gap** (GPIO 20)
- ✅ **Reverse P-3** (GPIO 37)

---

## [1.2.0] - 2026-01-25 (Internal Security Release)
- Refaktoryzacja przypisania pistoletów
- Dodanie funkcji Start Gap
- Zmiana obsługi przycisków

---

## [1.0.0] - 2026-01-25

### ✨ Pierwsza wersja produkcyjna

#### Dodano
- ✅ **Kompletny system 15 wzorców malowania**
  - P-1a do P-1e (wzorce przerywane)
  - P-2a, P-2b (wzorce ciągłe)
  - P-3a, P-3b (wzorce przekraczalne z funkcją odwracania)
  - P-4 (podwójna ciągła)
  - P-6 (ostrzegawcza)
  - P-7a do P-7d (krawędziowe)

- ✅ **System kalibracji enkodera**
  - Procedura kalibracji 10 metrów
  - Zapis współczynnika kalibracji w EEPROM
  - Automatyczne wczytywanie przy starcie
  - Walidacja poprawności kalibracji

- ✅ **Pomiar dystansu**
  - Dokładność ±1 cm
  - Pomiar prędkości w km/h
  - Licznik wymalowanych m²
  - Resetowanie liczników

- ✅ **Interfejs użytkownika**
  - Ekran główny pracy (wzorzec, prędkość, m²)
  - Menu systemowe (kalibracja, pomiar, ustawienia)
  - Ekran kalibracji z wizualizacją
  - Ekran pomiaru dystansu
  - Komunikaty systemowe

- ✅ **Sterowanie pistoletami**
  - 6 przekaźników sterujących pistoletami
  - Automatyczne włączanie/wyłączanie według wzorca
  - Synchronizacja z dystansem
  - Funkcja testowa

- ✅ **Obsługa przycisków**
  - Przycisk START/PAUZA
  - Przycisk STOP (krótkie/długie/bardzo długie)
  - Selektor (nawigacja w menu)
  - 15 przycisków wyboru wzorców
  - Debouncing i detekcja typu naciśnięcia

- ✅ **Zmiana wzorców "w locie"**
  - Natychmiastowa zmiana podczas malowania
  - Brak przerwy w pracy
  - Aktualizacja wyświetlacza

- ✅ **Dokumentacja**
  - Instrukcja obsługi (PL)
  - Dokumentacja techniczna
  - Schemat połączeń
  - Komentarze w kodzie

#### Techniczne
- Platforma: ESP32-S3 N16R8
- Framework: Arduino
- Wyświetlacz: ILI9341 320x240 (TFT_eSPI)
- Enkoder: KY-040 z obsługą przerwań
- Pamięć: EEPROM dla kalibracji
- Optymalizacja: -O2, 240 MHz CPU

#### Pliki
```
RoadPainter/
├── src/
│   ├── main.cpp              # Program główny
│   ├── config.h              # Konfiguracja pinów i stałych
│   ├── patterns.h/cpp        # System wzorców (15 wzorców)
│   ├── encoder.h/cpp         # Obsługa enkodera i pomiary
│   ├── relay_controller.h/cpp # Sterowanie przekaźnikami
│   ├── button_handler.h/cpp  # Obsługa przycisków
│   ├── display.h/cpp         # Interfejs wyświetlacza
│   └── ui_manager.h/cpp      # Zarządzanie UI i stanami
├── platformio.ini            # Konfiguracja projektu
└── README.md                 # Dokumentacja projektu

docs/
├── INSTRUKCJA_OBSLUGI.md     # Instrukcja dla operatora
├── DOKUMENTACJA_TECHNICZNA.md # Dokumentacja techniczna
├── SCHEMAT_POLACZEN.md       # Schemat elektroniczny
└── CHANGELOG.md              # Ten plik
```

#### Testowane
- ✅ Kompilacja bez błędów
- ✅ Wszystkie moduły zainicjalizowane poprawnie
- ✅ Logika wzorców (symulacja)
- ✅ System kalibracji
- ✅ Pomiar dystansu
- ✅ Interfejs użytkownika
- ✅ Sterowanie przekaźnikami

#### Znane ograniczenia
- Funkcja odwracania wzorców P-3 wymaga implementacji w menu ustawień
- Joystick nie jest obecnie wykorzystywany (zarezerwowany na przyszłość)
- Maksymalna prędkość ograniczona do 30 km/h (zabezpieczenie)

---

## Planowane wersje

### [1.1.0] - Planowane
- [ ] Menu ustawień z funkcją odwracania P-3
- [ ] Obsługa joysticka jako alternatywa dla selektora
- [ ] Zapis statystyk pracy (całkowite m², czas pracy)
- [ ] Tryb demo/test
- [ ] Kalibracja szerokości pistoletów

### [1.2.0] - Planowane
- [ ] Komunikacja Bluetooth (monitoring z telefonu)
- [ ] Eksport danych do pliku
- [ ] Wielojęzyczny interfejs (PL/EN)
- [ ] Automatyczne wykrywanie błędów

---

## Format wersji

Projekt używa [Semantic Versioning](https://semver.org/):
- **MAJOR**: Zmiany niekompatybilne wstecz
- **MINOR**: Nowe funkcje kompatybilne wstecz
- **PATCH**: Poprawki błędów

---

**Ostatnia aktualizacja**: 2026-01-25  
**Wersja**: 1.0.0

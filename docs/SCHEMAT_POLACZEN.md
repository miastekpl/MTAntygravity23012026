# SCHEMAT POŁĄCZEŃ ELEKTRYCZNYCH v1.6.0
## System Sterowania Malowarką Drogową "Trassar"

---

## ⚡ GŁÓWNA STRUKTURA ZASILANIA

| Sekcja | Napięcie | Źródło | Zastosowanie |
|--------|----------|--------|--------------|
| **LOGIKA** | 5V DC | Przetwornica DC/DC (LM2596) | ESP32, Wyświetlacz, Enkoder, Logika Przekaźników |
| **LOGIKA (Int)** | 3.3V DC | Stabilizator LDO ESP32 | Karta SD, Sygnały logiczne |
| **MOC** | 12V/24V DC | Akumulator Pojazdu | Cewki elektrozaworów (Pistolety) |

---

## 🔌 MAPA PINÓW (PINOUT) - ESP32-S3 N16R8

### Peryferia Główne

| Komponent | Pin ESP32 | Typ Sygnału | Uwagi |
|-----------|-----------|-------------|-------|
| **WYŚWIETLACZ TFT** (ILI9341) | | | |
| MOSI | GPIO 11 | SPI MOSI | Współdzielone z SD |
| MISO | GPIO 13 | SPI MISO | Współdzielone z SD |
| SCK | GPIO 12 | SPI SCK | Współdzielone z SD |
| CS | GPIO 10 | Output | Chip Select TFT |
| DC | GPIO 9 | Output | Data/Command |
| RST | GPIO 8 | Output | Reset |
| BL | GPIO 14 | PWM/Output | Podświetlenie (Backlight) |
| **ENKODER** (KY-040) | | | |
| CLK | GPIO 4 | Input (ISR) | Faza A (Podciąganie sprz.) |
| DT | GPIO 5 | Input | Faza B (Podciąganie sprz.) |
| SW | - | - | Nieużywany (Patrz E-STOP) |
| **KARTA SD** (MicroSD Module) | | | |
| CS | GPIO 43 | Output | Chip Select SD (NOWOŚĆ) |
| MOSI, MISO, SCK | 11, 13, 12 | SPI Bus | Równolegle do TFT |

### Sterowanie Wyjściami (Pistolety - Moduł Przekaźników 8-kanałowy)

| Pistolet | Pin ESP32 | Funkcja (Domyślna) | Szerokość |
|----------|-----------|--------------------|-----------|
| **P1** | GPIO 35 | Oś Jezdni (Lewy) | 12 cm |
| **P2** | GPIO 36 | Oś Jezdni (Środek) | 12 cm |
| **P3** | GPIO 33 | Oś Jezdni (Prawy) | 12 cm |
| **P4** | GPIO 34 | Oś (Szeroki) | 24 cm |
| **P5** | GPIO 26 | Krawędziowy (Wąski) | 12 cm |
| **P6** | GPIO 25 | Krawędziowy (Szeroki) | 24 cm |

> **UWAGA**: Moduły przekaźników często sterowane są "masą" (Low Level Trigger). Kod uwzględnia logikę sterowania. Sprawdź zworkę na module!

### Panel Sterowania (Przyciski - zwierać do GND)

**Wszystkie przyciski wymagają podłączenia jednej strony do Pinu ESP32, drugiej do MASY (GND). Piny są programowo podciągnięte do 3.3V (INPUT_PULLUP).**

| Sekcja | Nazwa Przycisku | Pin ESP32 | Opis Funkcji |
|--------|-----------------|-----------|--------------|
| **STEROWANIE** | START / PAUZA | GPIO 15 | Rozpoczęcie/Wstrzymanie malowania |
| | STOP | GPIO 16 | Zakończenie pracy i reset licznika |
| | MENU / SELEKTOR | GPIO 19 | Wejście do menu, zatwierdzanie |
| | **E-STOP** (Awaryjny) | GPIO 6 | **NC/NO** (Natychmiastowe odcięcie) |
| **FUNKCJE** | START GAP | GPIO 20 | Rozpoczęcie malowania od przerwy |
| | REVERSE P-3 | GPIO 37 | Zamiana stron P-3a <-> P-3b |
| **WYBÓR WZORCA** | P-1A (Przerywana) | GPIO 17 | |
| | P-1B (Przerywana) | GPIO 18 | |
| | ... (patrz kod) | ... | |
| | P-7D (Strzałka) | GPIO 7 | |

---

## 🛠️ SCHEMAT POŁĄCZEŃ (Instrukcja Montażu)

### 1. Zasilanie
1. Podłącz **Akumulator 12V/24V** do Wejścia Przetwornicy DC/DC (VIN).
2. Wyreguluj wyjście przetwornicy na **5.0V - 5.1V**.
3. Podłącz wyjście 5V do pinu `5V` (lub `VIN`) w ESP32-S3.
4. Połącz masę (GND) przetwornicy, ESP32, modułu przekaźników i enkodera w jednym punkcie (Wspólna Masa).

### 2. Wyświetlacz i SD (Magistrala SPI)
Połącz równolegle piny SCK, MISO, MOSI:
- `ESP32 GPIO 12` <-> `TFT SCK` ORAZ `SD SCK`
- `ESP32 GPIO 13` <-> `TFT MISO` ORAZ `SD MISO`
- `ESP32 GPIO 11` <-> `TFT MOSI` ORAZ `SD MOSI`
Piny CS (Chip Select) ODDZIELNIE:
- `ESP32 GPIO 10` -> `TFT CS`
- `ESP32 GPIO 43` -> `SD CS`
Pozostałe (TFT):
- `ESP32 GPIO 9` -> `TFT DC`
- `ESP32 GPIO 8` -> `TFT RST`
- `ESP32 GPIO 14` -> `TFT LED` (przez rezystor 100R opcjonalnie)

### 3. Pistolety (Przekaźniki)
- `ESP32 GPIO 35, 36...` -> Wejścia `IN1, IN2...` modułu przekaźników.
- `VCC` modułu przekaźników -> `5V`.
- `GND` modułu -> `GND`.
- Styki robocze (COM/NO) przekaźników wpinamy w obwód elektrozaworów (szeregowo z zasilaniem 12V/24V cewki).
  - `+12V Aku` -> `COM Przekaźnika`
  - `NO Przekaźnika` -> `+ Elektrozaworu`
  - `- Elektrozaworu` -> `Masa (GND) Aku`

### 4. E-STOP (Przycisk Awaryjny) - KRITICALNE!
Zalecamy użycie przycisku "Grzybek" z dwoma torami styków:
1. **Tor elektryczny (Hardware)**: Odcina zasilanie +12V/24V idące na COM przekaźników. To gwarantuje stop nawet przy awarii procesora.
2. **Tor sygnałowy (Software)**: Zwiera `GPIO 6` do `GND`. Informuje procesor o awarii (wyświetla komunikat, resetuje logikę).

---

## 📋 Lista Elementów (BOM)

1. **Mikrokontroler**: ESP32-S3 DevKitC-1 (Wersja N16R8) - 1 szt.
2. **Wyświetlacz**: 2.8" lub 3.2" TFT SPI ILI9341 (320x240) z dotykiem (opcja) - 1 szt.
3. **Enkoder**: Optyczny inkrementalny (np. OMRON E6B2) LUB magnetyczny (AS5600) LUB prosty KY-040 (dla testów) - 1 szt.
4. **Moduł Przekaźników**: 8-kanałowy 5V z optoizolacją - 1 szt.
5. **Moduł SD**: Czytnik MicroSD SPI (jeśli nie ma w wyświetlaczu) - 1 szt.
6. **Zasilanie**: Przetwornica LM2596 (Step-Down) - 1 szt.
7. **Obudowa**: Przemysłowa z szyną DIN (np. Kradex) - 1 szt.
8. **Złącza**: Skręcane ARK lub złącza lotnicze GX16 (dla enkodera/pistoletów).

---
Autor: Główny Inżynier Projektu
Wersja schematu: 1.6.0
Data: 2026-01-27

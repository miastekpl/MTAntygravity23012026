# System Malowania Pasów Drogowych
## Wersja 1.0.0

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP32-S3](https://img.shields.io/badge/Platform-ESP32--S3-blue.svg)](https://www.espressif.com/en/products/socs/esp32-s3)
[![Framework: Arduino](https://img.shields.io/badge/Framework-Arduino-00979D.svg)](https://www.arduino.cc/)

---

## 📋 Opis

Zaawansowany system automatycznego malowania pasów drogowych oparty na mikrokontrolerze **ESP32-S3**, obsługujący **15 wzorców** zgodnych z polskimi normami drogowymi.

### ✨ Główne funkcje:

- ✅ **15 wzorców malowania** (P-1a do P-7d)
- ✅ **Kalibracja enkodera** dla precyzyjnego pomiaru
- ✅ **Pomiar dystansu** z dokładnością ±1cm
- ✅ **Zmiana wzorców "w locie"** bez zatrzymywania
- ✅ **Automatyczne liczenie m²** wymalowanej powierzchni
- ✅ **Kolorowy wyświetlacz** 320x240 pikseli
- ✅ **Sterowanie 6 pistoletami** malarskimi

---

## 🛠️ Sprzęt

### Wymagane komponenty:

| Komponent | Model | Opis |
|-----------|-------|------|
| Mikrokontroler | ESP32-S3 N16R8 | 16MB Flash, 8MB PSRAM |
| Wyświetlacz | ILI9341 | TFT 320x240, SPI |
| Enkoder | KY-040 | Rotary encoder |
| Przekaźniki | 6× moduł | 12V z optronami |
| Przyciski | 17× | Monostabilne |
| Zasilanie | 12V DC | Z akumulatora pojazdu |

Pełna lista materiałów (BOM) w [SCHEMAT_POLACZEN.md](docs/SCHEMAT_POLACZEN.md)

---

## 📁 Struktura Projektu

```
MTAntygravity250126/
├── RoadPainter/              # Kod źródłowy
│   ├── src/
│   │   ├── main.cpp          # Program główny
│   │   ├── config.h          # Konfiguracja
│   │   ├── patterns.h/cpp    # Wzorce malowania
│   │   ├── encoder.h/cpp     # Obsługa enkodera
│   │   ├── relay_controller.h/cpp  # Przekaźniki
│   │   ├── button_handler.h/cpp    # Przyciski
│   │   ├── display.h/cpp     # Wyświetlacz
│   │   └── ui_manager.h/cpp  # Zarządzanie UI
│   └── platformio.ini        # Konfiguracja PlatformIO
│
├── docs/                     # Dokumentacja
│   ├── INSTRUKCJA_OBSLUGI.md
│   ├── DOKUMENTACJA_TECHNICZNA.md
│   ├── SCHEMAT_POLACZEN.md
│   └── CHANGELOG.md
│
└── release/                  # Paczki do wdrożenia
    └── RoadPainter_v1.0.0.zip
```

---

## 🚀 Instalacja

### Wymagania:

- [PlatformIO](https://platformio.org/) lub Arduino IDE
- Sterowniki USB dla ESP32-S3
- Kabel USB-C

### Kroki:

1. **Sklonuj repozytorium**:
   ```bash
   git clone https://github.com/miastekpl/MTAntygravity23012026.git
   cd MTAntygravity23012026/RoadPainter
   ```

2. **Otwórz w PlatformIO**:
   ```bash
   pio run
   ```

3. **Wgraj firmware**:
   ```bash
   pio run --target upload
   ```

4. **Monitor szeregowy**:
   ```bash
   pio device monitor
   ```

---

## 📖 Dokumentacja

- 📘 [Instrukcja Obsługi](docs/INSTRUKCJA_OBSLUGI.md) - Przewodnik dla operatora
- 📗 [Dokumentacja Techniczna](docs/DOKUMENTACJA_TECHNICZNA.md) - Szczegóły techniczne
- 📙 [Schemat Połączeń](docs/SCHEMAT_POLACZEN.md) - Pinout i montaż
- 📕 [Changelog](docs/CHANGELOG.md) - Historia zmian

---

## 🎨 Wzorce Malowania

System obsługuje 15 wzorców zgodnych z polskimi normami:

| Kod | Nazwa | Typ | Szerokość |
|-----|-------|-----|-----------|
| P-1a | Przerywana długa | 4m/8m | 12cm |
| P-1b | Przerywana krótka | 2m/4m | 12cm |
| P-1c | Wydzielająca | 2m/2m | 12cm |
| P-1d | Prowadząca wąska | 1m/1m | 12cm |
| P-1e | Prowadząca szeroka | 1m/1m | 24cm |
| P-2a | Ciągła wąska | Ciągła | 12cm |
| P-2b | Ciągła szeroka | Ciągła | 24cm |
| P-3a | Przekraczalna długa* | 4m/2m | 12cm |
| P-3b | Przekraczalna krótka* | 1m/1m | 12cm |
| P-4 | Podwójna ciągła* | Ciągła | 24cm |
| P-6 | Ostrzegawcza | 4m/2m | 12cm |
| P-7a | Krawędziowa przeryw. szer. | 1m/1m | 24cm |
| P-7b | Krawędziowa ciągła szer. | Ciągła | 24cm |
| P-7c | Krawędziowa przeryw. wąska | 1m/1m | 12cm |
| P-7d | Krawędziowa ciągła wąska | Ciągła | 12cm |

*Wzorce specjalne - szczegóły w instrukcji obsługi

---

## ⚙️ Konfiguracja

### Kalibracja (OBOWIĄZKOWA przed pierwszym użyciem):

1. Wejdź do **MENU** (przytrzymaj STOP 1s)
2. Wybierz **"Kalibracja"**
3. Naciśnij **START**
4. Przejedź **dokładnie 10 metrów**
5. Naciśnij **START** ponownie

Kalibracja zostanie zapisana w pamięci EEPROM.

---

## 🔧 Rozwój

### Kompilacja:

```bash
cd RoadPainter
pio run
```

### Upload:

```bash
pio run --target upload
```

### Monitor:

```bash
pio device monitor --baud 115200
```

### Clean:

```bash
pio run --target clean
```

---

## 📊 Dane Techniczne

- **Mikrokontroler**: ESP32-S3 @ 240MHz
- **Pamięć**: 16MB Flash, 8MB PSRAM
- **Wyświetlacz**: ILI9341 320x240 TFT
- **Dokładność pomiaru**: ±1 cm
- **Maksymalna prędkość**: 30 km/h
- **Zasilanie**: 12V DC
- **Temperatura pracy**: -10°C do +50°C

---

## 🤝 Wkład

Projekt jest otwarty na współpracę! Jeśli chcesz pomóc:

1. Fork repozytorium
2. Utwórz branch (`git checkout -b feature/NowaFunkcja`)
3. Commit zmian (`git commit -m 'Dodano nową funkcję'`)
4. Push do brancha (`git push origin feature/NowaFunkcja`)
5. Otwórz Pull Request

---

## 📝 Licencja

Ten projekt jest licencjonowany na licencji MIT - szczegóły w pliku [LICENSE](LICENSE).

---

## 👥 Autorzy

- **Road Painter Systems** - *Wersja początkowa* - [GitHub](https://github.com/miastekpl)

---

## 🐛 Zgłaszanie Błędów

Znalazłeś błąd? [Otwórz issue](https://github.com/miastekpl/MTAntygravity23012026/issues)

---

## 📞 Kontakt

- 📧 Email: support@roadpainter.pl
- 🌐 GitHub: [MTAntygravity23012026](https://github.com/miastekpl/MTAntygravity23012026)

---

## ⭐ Podziękowania

- Espressif za ESP32-S3
- Bodmer za bibliotekę TFT_eSPI
- Społeczność Arduino/PlatformIO

---

**Wersja**: 1.0.0  
**Data wydania**: 2026-01-25  
**Status**: ✅ Produkcyjny

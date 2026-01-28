# INSTRUKCJA OBSŁUGI - SYSTEM TRASSAR v1.6.0
## Inteligentny System Sterowania Malowarką Drogową

---

## 🏗️ Wstęp
Gratulujemy wyboru systemu sterowania **Trassar v1.6.0**. Jest to rozwiązanie klasy przemysłowej (Industrial Grade), zaprojektowane z myślą o niezawodności, precyzji i bezpieczeństwie operatora.

### Główne Funkcje:
- **15 Wzorców** zgodnych z polskimi normami (P-1, P-2, P-3, P-4, P-6, P-7).
- **Zdalne Sterowanie (IoT)**: Pełna kontrola ze smartfona/tabletu przez WiFi.
- **Czarna Skrzynka**: Rejestracja pracy na karcie SD (raporty dla inwestora).
- **Bezpieczeństwo**: Sprzętowy wyłącznik E-STOP i Watchdog Timer.
- **Precyzja**: Pomiar dystansu z dokładnością enkodera.

---

## 🚀 Pierwsze Uruchomienie

### 1. Zasilanie
System uruchamia się automatycznie po podaniu zasilania.
- Ekran powitalny wyświetli wersję oprogramowania (v1.6.0).
- Po 2 sekundach pojawi się **Ekran Główny**.

### 2. Kalibracja Enkodera (Wymagane przy pierwszym razie)
Jeśli na ekranie miga komunikat **"WYMAGANA KALIBRACJA!"**:
1. Upewnij się, że maszyna stoi w miejscu startu (oznacz linię na asfalcie).
2. Przytrzymaj przycisk **STOP** (3 sekundy) aby wejść do MENU.
3. Wybierz opcję **KALIBRACJA** używając przycisków wzorców (Góra/Dół) lub Selektora.
4. Potwierdź Selektorem.
5. Przejedź dokładnie **10 metrów** (użyj taśmy mierniczej).
6. Zatrzymaj się i naciśnij **Selektor**.
7. System zapamięta ustawienia na zawsze (technologia NVS).

---

## 📱 Zdalne Sterowanie (Web Dashboard)

System tworzy własną sieć WiFi, umożliwiając sterowanie z kabiny pojazdu.

### Jak się połączyć?
1. Weź Swój smartfon lub tablet.
2. Włącz WiFi i wyszukaj sieć: **Trassar**.
3. Hasło: **12345678**.
4. W przeglądarce wpisz adres: `192.168.4.1`.

### Funkcje Dashboardu:
- **Status Na Żywo**: Prędkość malowania, przebyty dystans, ilość m² farby.
- **Wielkie Przyciski**: START / PAUZA / STOP (łatwe trafienie palcem).
- **Wybór Wzorca**: Lista wszystkich 15 wzorców. Kliknij, aby zmienić.
- **Funkcje Specjalne**:
  - `START GAP` - Rozpocznij malowanie od przerwy (przydatne przy odświeżaniu).
  - `ODWRÓĆ P-3` - Zamień linię ciągłą z przerywaną stronami.

### ⚠️ Aktualizacja Systemu (OTA)
Aby zaktualizować oprogramowanie bez kabla:
1. Wejdź na adres: `192.168.4.1/update`.
2. Wybierz plik `firmware.bin` (dostarczony przez producenta).
3. Kliknij "Update". System zrestartuje się po zakończeniu.

---

## 💾 Raporty Pracy (Karta SD)

System automatycznie zapisuje historię pracy na karcie SD w formacie CSV (Excel).
Karta znajduje się w slocie obok wyświetlacza.

### Struktura Pliku (LOG_XXX.CSV):
| Czas (ms) | Zdarzenie | Wzorzec | Dystans (m) | Powierzchnia (m²) | Prędkość (km/h) |
|-----------|-----------|---------|:-----------:|:-----------------:|:---------------:|
| 105420 | START_WORK | P-1a | 0.00 | 0.000 | 0.0 |
| 125500 | LOG_WORK | P-1a | 50.50 | 0.606 | 4.2 |
| 145000 | STOP_WORK | P-1a | 100.00 | 1.212 | 0.0 |

**Wskazówka:** Pliki są automatycznie numerowane (`LOG_1.CSV`, `LOG_2.CSV`...), więc nie nadpiszesz starych danych. Możesz oddać kartę SD inwestorowi jako dowód wykonania zlecenia.

---

## 🚧 Sytuacje Awaryjne

### Użycie E-STOP
W przypadku zagrożenia (np. wyciek farby, wtargnięcie pieszego):
1. Uderz w czerwony przycisk **"Grzybek"** (E-STOP).
2. **Efekt Natychmiastowy**: Wszystkie pistolety zostaną zamknięte sprzętowo. Zasilanie elektrozaworów zostanie odcięte.
3. System wyświetli czerwony ekran "E-STOP ZATRZYMANIE AWARYJNE".

### Reset po awarii
Aby wznowić pracę:
1. Odblokuj przycisk grzybka (przekręć).
2. Wyłącz i włącz zasilanie sterownika "Trassar" (wymagany pełny restart dla bezpieczeństwa).

---

## 🔧 Rozwiązywanie Problemów

| Objaw | Przyczyna | Rozwiązanie |
|-------|-----------|-------------|
| **Ekran nie świeci** | Brak zasilania 5V | Sprawdź przetwornicę i bezpiecznik. |
| **Prędkość skacze** | Luźne koło/enkoder | Dokręć mocowanie enkodera do koła. |
| **Brak WiFi "Trassar"** | Antena zasłonięta | Wyprowadź antenę ESP32 poza metalową obudowę. |
| **Brak logów SD** | Karta pełna/błąd | Sformatuj kartę SD (FAT32) i włóż ponownie. |
| **Pistolety nie strzelają** | Zasilanie 12V | Sprawdź bezpiecznik na linii zasilania elektrozaworów. |

---

Autor: Dział Wsparcia Technicznego
Kontakt: serwis@trassar.pl
Data wydania: 27.01.2026

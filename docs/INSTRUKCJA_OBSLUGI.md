# INSTRUKCJA OBSŁUGI
## System Malowania Pasów Drogowych v1.0.0

---

## 1. WPROWADZENIE

System automatycznego malowania pasów drogowych to zaawansowane urządzenie sterowane mikrokontro lerem ESP32-S3, przeznaczone do precyzyjnego nakładania 15 różnych wzorców zgodnych z polskimi normami drogowymi.

### Główne funkcje:
- ✅ 15 wzorców malowania (P-1a do P-7d)
- ✅ Kalibracja enkodera dla precyzyjnego pomiaru
- ✅ Pomiar dystansu z dokładnością 1cm
- ✅ Zmiana wzorców podczas pracy
- ✅ Automatyczne liczenie wymalowanych m²
- ✅ Wyświetlacz kolorowy 320x240 pikseli

---

## 2. PANEL STEROWANIA

### Przyciski sterowania:
- **START/PAUZA** - rozpoczęcie/wstrzymanie malowania
- **STOP** - zatrzymanie pracy / wejście do menu
- **SELEKTOR** - nawigacja w menu

### Przyciski wzorców (15 sztuk):
- **P-1a** do **P-1e** - wzorce przerywane
- **P-2a**, **P-2b** - wzorce ciągłe
- **P-3a**, **P-3b** - wzorce przekraczalne
- **P-4** - podwójna ciągła
- **P-6** - ostrzegawcza
- **P-7a** do **P-7d** - krawędziowe

### Wyświetlacz:
Ekran główny pokazuje:
- **Lewy górny róg**: Symbol wybranego wzorca
- **Prawy górny róg**: Prędkość pojazdu [km/h]
- **Środek**: Wymalowane m²
- **Dół**: Status (MALOWANIE / PAUZA)

---

## 3. PIERWSZE URUCHOMIENIE

### Krok 1: Kalibracja enkodera (OBOWIĄZKOWA!)

> [!IMPORTANT]
> Kalibracja musi być wykonana przed pierwszym użyciem systemu!

1. Przytrzymaj przycisk **STOP** przez **1 sekundę**
2. Wybierz **"Kalibracja"** selektorem (krótkie naciśnięcia)
3. Potwierdź długim naciśnięciem selektora
4. Naciśnij **START**
5. **Przejedź dokładnie 10 metrów** (zmierz taśmą mierniczą)
6. Zatrzymaj się i naciśnij **START** ponownie
7. System zapisze kalibrację automatycznie

✅ Kalibracja jest zapisywana w pamięci i nie wymaga powtarzania

---

## 4. PODSTAWOWA OBSŁUGA

### Rozpoczęcie malowania:

1. **Wybierz wzorzec** - naciśnij jeden z 15 przycisków wzorców
   - Na wyświetlaczu pojawi się nazwa wzorca (np. "P-1a")
   
2. **Rozpocznij malowanie** - naciśnij **START/PAUZA**
   - Status zmieni się na ">>> MALOWANIE <<<"
   - Pistolety malarskie włączą się automatycznie
   
3. **Jedź z odpowiednią prędkością**
   - Prędkość wyświetlana na ekranie
   - Zalecana prędkość: 3-8 km/h
   
4. **Obserwuj licznik m²**
   - System automatycznie liczy wymalowaną powierzchnię

### Pauza w pracy:

- Naciśnij **START/PAUZA** - malowanie zostanie wstrzymane
- Pistolety wyłączą się automatycznie
- Ponowne naciśnięcie **START/PAUZA** wznowi pracę

### Zatrzymanie:

- Naciśnij krótko **STOP** - zatrzymanie malowania
- Licznik m² zostanie zachowany

---

## 5. ZMIANA WZORCA "W LOCIE"

> [!TIP]
> Możesz zmienić wzorzec podczas malowania bez zatrzymywania!

1. Podczas malowania naciśnij przycisk **nowego wzorca**
2. System natychmiast przełączy się na nowy wzorzec
3. Malowanie kontynuowane bez przerwy

**Przykład**: Malujesz wzorcem P-4, naciśnij przycisk P-3b → natychmiastowa zmiana

---

## 6. WZORCE PRZEKRACZALNE (P-3a, P-3b)

Wzorce P-3a i P-3b mają specjalną funkcję **odwracania**:

- **Normalnie**: Linia ciągła po lewej, przerywana po prawej
- **Po odwróceniu**: Linia ciągła po prawej, przerywana po lewej

### Jak odwrócić:
1. Wybierz wzorzec P-3a lub P-3b
2. Wejdź do **MENU** → **Ustawienia**
3. Wybierz **"Odwróć P-3"**
4. Potwierdź selektorem

---

## 7. POMIAR DYSTANSU

Funkcja pomiaru pozwala zmierzyć dokładny dystans przejazdu.

### Procedura:

1. Przytrzymaj **STOP** przez 1 sekundę (wejście do menu)
2. Wybierz **"Pomiar dystansu"**
3. Potwierdź długim naciśnięciem selektora
4. Naciśnij **START** - pomiar rozpocznie się
5. Jedź dowolny dystans
6. Zatrzymaj się - na ekranie zobaczysz dokładny dystans w metrach

**Dokładność**: ±1 cm

### Wyjście z pomiaru:
- Przytrzymaj **STOP** przez **2 sekundy**

---

## 8. MENU SYSTEMOWE

### Wejście do menu:
- Przytrzymaj **STOP** przez **1 sekundę** (tylko gdy nie malujesz)

### Pozycje menu:

#### 📏 Kalibracja
- Kalibracja enkodera (10 metrów)
- Wymagana przy pierwszym uruchomieniu

#### 📐 Pomiar dystansu
- Precyzyjny pomiar przejazdu
- Dokładność 1 cm

#### ⚙️ Ustawienia
- Odwracanie wzorców P-3
- Inne ustawienia (w przygotowaniu)

#### ↩️ Powrót
- Powrót do ekranu głównego

### Nawigacja w menu:
- **Krótkie naciśnięcie selektora** - przejście do następnej pozycji
- **Długie naciśnięcie selektora** - wybór pozycji
- **Przytrzymanie STOP (2s)** - wyjście do ekranu głównego

---

## 9. TABELA WZORCÓW

| Wzorzec | Linia (m) | Przerwa (m) | Szerokość (cm) | Opis |
|---------|-----------|-------------|----------------|------|
| **P-1a** | 4.0 | 8.0 | 12 | Przerywana długa |
| **P-1b** | 2.0 | 4.0 | 12 | Przerywana krótka |
| **P-1c** | 2.0 | 2.0 | 12 | Wydzielająca |
| **P-1d** | 1.0 | 1.0 | 12 | Prowadząca wąska |
| **P-1e** | 1.0 | 1.0 | 24 | Prowadząca szeroka |
| **P-2a** | Ciągła | - | 12 | Ciągła wąska |
| **P-2b** | Ciągła | - | 24 | Ciągła szeroka |
| **P-3a** | 4.0 | 2.0 | 12 | Przekraczalna długa* |
| **P-3b** | 1.0 | 1.0 | 12 | Przekraczalna krótka* |
| **P-4** | Ciągła | - | 24 | Podwójna ciągła* |
| **P-6** | 4.0 | 2.0 | 12 | Ostrzegawcza |
| **P-7a** | 1.0 | 1.0 | 24 | Krawędziowa przeryw. szer. |
| **P-7b** | Ciągła | - | 24 | Krawędziowa ciągła szer. |
| **P-7c** | 1.0 | 1.0 | 12 | Krawędziowa przeryw. wąska |
| **P-7d** | Ciągła | - | 12 | Krawędziowa ciągła wąska |

*Wzorce specjalne - patrz sekcje powyżej

---

## 10. ROZWIĄZYWANIE PROBLEMÓW

### ⚠️ "Wymagana kalibracja"
**Przyczyna**: Enkoder nie został skalibrowany  
**Rozwiązanie**: Wykonaj kalibrację (sekcja 3)

### ⚠️ "Wybierz wzorzec"
**Przyczyna**: Nie wybrano wzorca przed startem  
**Rozwiązanie**: Naciśnij jeden z przycisków wzorców

### ⚠️ Nieprawidłowe pomiary dystansu
**Przyczyna**: Błędna kalibracja  
**Rozwiązanie**: Powtórz kalibrację

### ⚠️ Pistolety nie włączają się
**Przyczyna**: 
1. Brak zasilania przekaźników
2. Uszkodzony przekaźnik

**Rozwiązanie**: Sprawdź połączenia elektryczne

### ⚠️ Wyświetlacz nie działa
**Przyczyna**: Brak zasilania lub uszkodzony wyświetlacz  
**Rozwiązanie**: Sprawdź połączenia SPI

---

## 11. KONSERWACJA

### Codziennie:
- ✅ Sprawdź czystość enkodera
- ✅ Sprawdź połączenia elektryczne

### Co tydzień:
- ✅ Sprawdź kalibrację (pomiar testowy 10m)
- ✅ Wyczyść wyświetlacz

### Co miesiąc:
- ✅ Sprawdź wszystkie przekaźniki (test sekwencyjny)
- ✅ Sprawdź stan przycisków

---

## 12. DANE TECHNICZNE

- **Mikrokontroler**: ESP32-S3 N16R8
- **Wyświetlacz**: ILI9341 320x240 TFT
- **Enkoder**: KY-040 rotary encoder
- **Przekaźniki**: 6 sztuk (sterowanie pistoletami)
- **Zasilanie**: 12V DC
- **Temperatura pracy**: -10°C do +50°C
- **Dokładność pomiaru**: ±1 cm
- **Maksymalna prędkość**: 30 km/h

---

## 13. BEZPIECZEŃSTWO

> [!CAUTION]
> - Nie używaj systemu podczas deszczu bez osłony
> - Nie przekraczaj maksymalnej prędkości 30 km/h
> - Regularnie sprawdzaj stan połączeń elektrycznych
> - Nie modyfikuj oprogramowania bez konsultacji

---

## 14. KONTAKT I WSPARCIE

W razie pytań lub problemów:
- 📧 Email: support@roadpainter.pl
- 📞 Telefon: +48 XXX XXX XXX
- 🌐 GitHub: https://github.com/miastekpl/MTAntygravity23012026

---

**Wersja dokumentu**: 1.0.0  
**Data**: 2026-01-25  
**Producent**: Road Painter Systems

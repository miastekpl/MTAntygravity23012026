#include "display.h"
#include <string.h>

// ============================================================================
// KONSTRUKTOR
// ============================================================================

DisplayManager::DisplayManager()
    : tft()
    , currentState(SCREEN_WORK)
    , lastState(SCREEN_WORK)
    , selectedMenuItem(MENU_CALIBRATION)
    , needsFullRedraw(true)
    , lastSpeed(-1.0)
    , lastArea(-1.0)
    , lastPaintingState(false)
{
    memset(lastPatternName, 0, sizeof(lastPatternName));
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

void DisplayManager::begin() {
    tft.init();
    tft.setRotation(3);  // Landscape
    tft.fillScreen(COLOR_BG);
    
    // Włączenie podświetlenia
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    
    Serial.println("DisplayManager: Inicjalizacja zakończona");
    
    // Wyświetlenie ekranu powitalnego
    tft.setTextColor(COLOR_HEADER, COLOR_BG);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("SYSTEM MALOWANIA", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 30, 4);
    tft.drawString("PASOW DROGOWYCH", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 4);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.drawString("v1.0.0", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 40, 2);
    delay(2000);
    
    clear();
    needsFullRedraw = true;
}

// ============================================================================
// ZMIANA STANU EKRANU
// ============================================================================

void DisplayManager::setState(ScreenState state) {
    if (currentState != state) {
        lastState = currentState;
        currentState = state;
        needsFullRedraw = true;
        clear();
    }
}

// ============================================================================
// AKTUALIZACJA WYŚWIETLACZA
// ============================================================================

void DisplayManager::update() {
    if (needsFullRedraw) {
        switch (currentState) {
            case SCREEN_WORK:
                drawWorkScreen();
                break;
            case SCREEN_MENU:
                drawMenu();
                break;
            case SCREEN_CALIBRATION:
                drawCalibrationScreen();
                break;
            case SCREEN_DISTANCE_MEASURE:
                drawDistanceMeasureScreen();
                break;
            default:
                break;
        }
        needsFullRedraw = false;
    }
}

// ============================================================================
// EKRAN PRACY
// ============================================================================

void DisplayManager::updateWorkScreen(const char* patternName, float speed_kmh, float area_m2, bool isPainting) {
    // Sprawdzenie czy coś się zmieniło
    bool changed = false;
    
    if (strcmp(lastPatternName, patternName) != 0) {
        strcpy(lastPatternName, patternName);
        changed = true;
    }
    
    if (abs(lastSpeed - speed_kmh) > 0.1) {
        lastSpeed = speed_kmh;
        changed = true;
    }
    
    if (abs(lastArea - area_m2) > 0.01) {
        lastArea = area_m2;
        changed = true;
    }
    
    if (lastPaintingState != isPainting) {
        lastPaintingState = isPainting;
        changed = true;
    }
    
    if (changed || needsFullRedraw) {
        drawWorkScreen();
    }
}

void DisplayManager::drawWorkScreen() {
    tft.fillScreen(COLOR_BG);
    
    // Nagłówek
    tft.fillRect(0, 0, SCREEN_WIDTH, 40, COLOR_HEADER);
    tft.setTextColor(COLOR_BG, COLOR_HEADER);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("PRACA", SCREEN_WIDTH/2, 20, 4);
    
    // Wzorzec (lewy górny róg)
    tft.setTextColor(COLOR_WARNING, COLOR_BG);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("WZORZEC:", 10, 50, 2);
    tft.setTextColor(COLOR_ACTIVE, COLOR_BG);
    tft.drawString(lastPatternName, 10, 75, 4);
    
    // Prędkość (prawy górny)
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("PREDKOSC:", SCREEN_WIDTH - 10, 50, 2);
    
    char speedStr[16];
    sprintf(speedStr, "%.1f", lastSpeed);
    tft.setTextColor(COLOR_ACTIVE, COLOR_BG);
    tft.setTextDatum(TR_DATUM);
    tft.drawString(speedStr, SCREEN_WIDTH - 50, 75, 6);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.drawString("km/h", SCREEN_WIDTH - 10, 95, 2);
    
    // Wymalowane m² (środek-dół)
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("WYMALOWANE:", SCREEN_WIDTH/2, 140, 2);
    
    char areaStr[16];
    sprintf(areaStr, "%.2f", lastArea);
    tft.setTextColor(COLOR_ACTIVE, COLOR_BG);
    tft.drawString(areaStr, SCREEN_WIDTH/2, 170, 6);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.drawString("m2", SCREEN_WIDTH/2, 210, 4);
    
    // Status (dół)
    tft.setTextDatum(MC_DATUM);
    if (lastPaintingState) {
        tft.setTextColor(COLOR_ACTIVE, COLOR_BG);
        tft.drawString(">>> MALOWANIE <<<", SCREEN_WIDTH/2, SCREEN_HEIGHT - 15, 2);
    } else {
        tft.setTextColor(COLOR_INACTIVE, COLOR_BG);
        tft.drawString("PAUZA", SCREEN_WIDTH/2, SCREEN_HEIGHT - 15, 2);
    }
}

// ============================================================================
// MENU
// ============================================================================

void DisplayManager::updateMenu(MenuPosition selectedPos) {
    selectedMenuItem = selectedPos;
    drawMenu();
}

void DisplayManager::nextMenuItem() {
    selectedMenuItem = (MenuPosition)((selectedMenuItem + 1) % MENU_COUNT);
    drawMenu();
}

void DisplayManager::prevMenuItem() {
    if (selectedMenuItem == 0) {
        selectedMenuItem = (MenuPosition)(MENU_COUNT - 1);
    } else {
        selectedMenuItem = (MenuPosition)(selectedMenuItem - 1);
    }
    drawMenu();
}

void DisplayManager::drawMenu() {
    tft.fillScreen(COLOR_BG);
    
    // Nagłówek
    drawHeader("MENU");
    
    const char* menuItems[] = {
        "Kalibracja",
        "Pomiar dystansu",
        "Ustawienia",
        "Powrot"
    };
    
    int startY = 60;
    int itemHeight = 40;
    
    for (int i = 0; i < MENU_COUNT; i++) {
        int y = startY + i * itemHeight;
        
        if (i == selectedMenuItem) {
            // Podświetlenie wybranej pozycji
            tft.fillRect(10, y - 5, SCREEN_WIDTH - 20, itemHeight - 5, COLOR_MENU_SELECT);
            tft.setTextColor(COLOR_TEXT, COLOR_MENU_SELECT);
        } else {
            tft.setTextColor(COLOR_TEXT, COLOR_BG);
        }
        
        tft.setTextDatum(ML_DATUM);
        tft.drawString(menuItems[i], 30, y + 10, 4);
    }
}

// ============================================================================
// EKRAN KALIBRACJI
// ============================================================================

void DisplayManager::updateCalibrationScreen(bool started, float distance_cm) {
    tft.fillScreen(COLOR_BG);
    drawHeader("KALIBRACJA");
    
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.setTextDatum(MC_DATUM);
    
    if (!started) {
        tft.drawString("Nacisnij START", SCREEN_WIDTH/2, 80, 4);
        tft.drawString("Przejedz 10 metrow", SCREEN_WIDTH/2, 120, 2);
        tft.drawString("Nacisnij START ponownie", SCREEN_WIDTH/2, 150, 2);
    } else {
        tft.drawString("KALIBRACJA W TOKU", SCREEN_WIDTH/2, 80, 4);
        
        char distStr[32];
        sprintf(distStr, "%.2f m", distance_cm / 100.0);
        tft.setTextColor(COLOR_ACTIVE, COLOR_BG);
        tft.drawString(distStr, SCREEN_WIDTH/2, 130, 6);
        
        tft.setTextColor(COLOR_WARNING, COLOR_BG);
        tft.drawString("Zatrzymaj sie na 10.00m", SCREEN_WIDTH/2, 190, 2);
    }
}

// ============================================================================
// EKRAN POMIARU DYSTANSU
// ============================================================================

void DisplayManager::updateDistanceMeasureScreen(bool measuring, float distance_cm) {
    tft.fillScreen(COLOR_BG);
    drawHeader("POMIAR DYSTANSU");
    
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.setTextDatum(MC_DATUM);
    
    if (!measuring) {
        tft.drawString("Nacisnij START", SCREEN_WIDTH/2, 100, 4);
        tft.drawString("aby rozpoczac pomiar", SCREEN_WIDTH/2, 140, 2);
    } else {
        tft.drawString("POMIAR", SCREEN_WIDTH/2, 70, 4);
        
        char distStr[32];
        sprintf(distStr, "%.2f", distance_cm / 100.0);
        tft.setTextColor(COLOR_ACTIVE, COLOR_BG);
        tft.drawString(distStr, SCREEN_WIDTH/2, 130, 6);
        tft.setTextColor(COLOR_TEXT, COLOR_BG);
        tft.drawString("metrow", SCREEN_WIDTH/2, 180, 4);
    }
}

// ============================================================================
// KOMUNIKATY
// ============================================================================

void DisplayManager::showMessage(const char* title, const char* message, uint16_t color) {
    tft.fillScreen(COLOR_BG);
    
    tft.setTextColor(color, COLOR_BG);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(title, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 30, 4);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.drawString(message, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 20, 2);
    
    delay(2000);
    needsFullRedraw = true;
}

// ============================================================================
// POMOCNICZE FUNKCJE
// ============================================================================

void DisplayManager::drawHeader(const char* title) {
    tft.fillRect(0, 0, SCREEN_WIDTH, 40, COLOR_HEADER);
    tft.setTextColor(COLOR_BG, COLOR_HEADER);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(title, SCREEN_WIDTH/2, 20, 4);
}

void DisplayManager::clear() {
    tft.fillScreen(COLOR_BG);
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

DisplayManager displayManager;

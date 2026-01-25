#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"
#include "patterns.h"

// ============================================================================
// STANY EKRANU
// ============================================================================

enum ScreenState {
    SCREEN_WORK = 0,           // Ekran pracy
    SCREEN_MENU,               // Menu główne
    SCREEN_CALIBRATION,        // Kalibracja
    SCREEN_DISTANCE_MEASURE,   // Pomiar dystansu
    SCREEN_SETTINGS            // Ustawienia
};

// ============================================================================
// POZYCJE MENU
// ============================================================================

enum MenuPosition {
    MENU_CALIBRATION = 0,
    MENU_DISTANCE_MEASURE,
    MENU_SETTINGS,
    MENU_BACK,
    MENU_COUNT
};

// ============================================================================
// KLASA OBSŁUGI WYŚWIETLACZA
// ============================================================================

class DisplayManager {
public:
    DisplayManager();
    
    // Inicjalizacja
    void begin();
    
    // Zmiana stanu ekranu
    void setState(ScreenState state);
    ScreenState getState() const { return currentState; }
    
    // Aktualizacja wyświetlacza
    void update();
    
    // Ekran pracy
    void updateWorkScreen(const char* patternName, float speed_kmh, float area_m2, bool isPainting);
    
    // Menu
    void updateMenu(MenuPosition selectedPos);
    void nextMenuItem();
    void prevMenuItem();
    MenuPosition getSelectedMenuItem() const { return selectedMenuItem; }
    
    // Ekran kalibracji
    void updateCalibrationScreen(bool started, float distance_cm);
    
    // Ekran pomiaru dystansu
    void updateDistanceMeasureScreen(bool measuring, float distance_cm);
    
    // Komunikaty
    void showMessage(const char* title, const char* message, uint16_t color = COLOR_TEXT);
    
    // Wyczyszczenie ekranu
    void clear();
    
private:
    TFT_eSPI tft;
    ScreenState currentState;
    ScreenState lastState;
    MenuPosition selectedMenuItem;
    
    // Flagi odświeżania
    bool needsFullRedraw;
    
    // Ostatnie wartości (dla optymalizacji)
    char lastPatternName[16];
    float lastSpeed;
    float lastArea;
    bool lastPaintingState;
    
    // Rysowanie poszczególnych ekranów
    void drawWorkScreen();
    void drawMenu();
    void drawCalibrationScreen();
    void drawDistanceMeasureScreen();
    
    // Pomocnicze funkcje rysowania
    void drawHeader(const char* title);
    void drawCenteredText(const char* text, int y, uint8_t font, uint16_t color = COLOR_TEXT);
    void drawBigNumber(float value, int x, int y, const char* unit, uint16_t color = COLOR_TEXT);
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern DisplayManager displayManager;

#endif // DISPLAY_H

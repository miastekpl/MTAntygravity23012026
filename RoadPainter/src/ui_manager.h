#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "patterns.h"
#include "encoder.h"

// ============================================================================
// STANY SYSTEMU
// ============================================================================

enum SystemState {
    STATE_IDLE = 0,        // Gotowość
    STATE_PAINTING,        // Malowanie
    STATE_PAUSED,          // Pauza
    STATE_MENU,            // Menu
    STATE_CALIBRATION,     // Kalibracja
    STATE_DISTANCE_MEASURE // Pomiar dystansu
};

// ============================================================================
// KLASA ZARZĄDZANIA INTERFEJSEM
// ============================================================================

class UIManager {
public:
    UIManager();
    
    // Inicjalizacja
    void begin();
    
    // Aktualizacja (wywołuj w loop)
    void update();
    
    // Obsługa przycisków
    void handleStartPause();
    void handleStop(PressType pressType);
    void handleSelector(PressType pressType);
    void handlePatternButton(PatternType pattern);
    
    // Pobieranie stanu
    SystemState getState() const { return currentState; }
    bool isPainting() const { return currentState == STATE_PAINTING; }
    
    // Sprawdzanie czy wzorzec jest aktywny
    bool isPatternActive() const { return patternManager.isPatternActive(); }
    
    // Gettery dla WebServer
    SystemState getState() const { return currentState; }
    float getTotalArea() const { return totalPaintedArea_m2; }
    
private:
    SystemState currentState;
    
    // Dane dla ekranu pracy
    float totalPaintedArea_m2;
    
    // Kalibracja
    bool calibrationStarted;
    float calibrationStartDistance;
    
    // Pomiar dystansu
    bool distanceMeasureStarted;
    float distanceMeasureStart;
    
    // Zmiana stanu
    void setState(SystemState newState);
    
    // Aktualizacja ekranów
    void updateWorkScreen();
    void updateMenuScreen();
    void updateCalibrationScreen();
    void updateDistanceMeasureScreen();
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern UIManager uiManager;

#endif // UI_MANAGER_H

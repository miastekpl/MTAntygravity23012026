#include "ui_manager.h"
#include "relay_controller.h"
#include "data_logger.h"

// ============================================================================
// KONSTRUKTOR
// ============================================================================

UIManager::UIManager()
    : currentState(STATE_IDLE)
    , totalPaintedArea_m2(0.0)
    , calibrationStarted(false)
    , calibrationStartDistance(0.0)
    , distanceMeasureStarted(false)
    , distanceMeasureStart(0.0)
{
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

void UIManager::begin() {
    setState(STATE_IDLE);
    Serial.println("UIManager: Inicjalizacja zakończona");
}

// ============================================================================
// ZMIANA STANU
// ============================================================================

void UIManager::setState(SystemState newState) {
    if (currentState != newState) {
        Serial.printf("UIManager: Zmiana stanu %d -> %d\n", currentState, newState);
        currentState = newState;
        
        // Aktualizacja ekranu
        switch (newState) {
            case STATE_IDLE:
            case STATE_PAINTING:
            case STATE_PAUSED:
                displayManager.setState(SCREEN_WORK);
                break;
            case STATE_MENU:
                displayManager.setState(SCREEN_MENU);
                break;
            case STATE_CALIBRATION:
                displayManager.setState(SCREEN_CALIBRATION);
                calibrationStarted = false;
                break;
            case STATE_DISTANCE_MEASURE:
                displayManager.setState(SCREEN_DISTANCE_MEASURE);
                distanceMeasureStarted = false;
                break;
        }
    }
}

// ============================================================================
// AKTUALIZACJA
// ============================================================================

void UIManager::update() {
    switch (currentState) {
        case STATE_IDLE:
        case STATE_PAINTING:
        case STATE_PAUSED:
            updateWorkScreen();
            break;
        case STATE_MENU:
            updateMenuScreen();
            break;
        case STATE_CALIBRATION:
            updateCalibrationScreen();
            break;
        case STATE_DISTANCE_MEASURE:
            updateDistanceMeasureScreen();
            break;
    }
}

// ============================================================================
// AKTUALIZACJA EKRANU PRACY
// ============================================================================

void UIManager::updateWorkScreen() {
    // Aktualizacja wzorca malowania
    if (currentState == STATE_PAINTING) {
        float deltaDist = encoderManager.getDeltaDistance_cm();
        float currentSpeed = encoderManager.getSpeed_kmh();
        
        // ZABEZPIECZENIE: Pistolety aktywne tylko przy ruchu i prędkości >= 2 km/h
        if (currentSpeed >= MIN_SPEED_FOR_PAINTING && deltaDist > 0.0) {
            patternManager.update(deltaDist);
            
            // Aktualizacja powierzchni
            totalPaintedArea_m2 += patternManager.calculatePaintedArea(deltaDist);
            
            // Aktualizacja przekaźników
            relayController.setGunMask(patternManager.getActiveGunMask());
        } else {
            // Prędkość za niska lub brak ruchu - wyłącz pistolety
            relayController.allOff();
            
            if (currentSpeed < MIN_SPEED_FOR_PAINTING && currentSpeed > 0.1) {
                Serial.printf("UWAGA: Prędkość za niska (%.1f km/h) - pistolety wyłączone\n", currentSpeed);
            }
        }
    } else {
        // Wyłączenie pistoletów gdy nie malujemy
        relayController.allOff();
    }
    
    // Aktualizacja wyświetlacza
    displayManager.updateWorkScreen(
        patternManager.getPatternName(),
        encoderManager.getSpeed_kmh(),
        totalPaintedArea_m2,
        currentState == STATE_PAINTING
    );
}

// ============================================================================
// AKTUALIZACJA MENU
// ============================================================================

void UIManager::updateMenuScreen() {
    displayManager.updateMenu(displayManager.getSelectedMenuItem());
}

// ============================================================================
// AKTUALIZACJA EKRANU KALIBRACJI
// ============================================================================

void UIManager::updateCalibrationScreen() {
    float currentDistance = encoderManager.getDistanceCm();
    displayManager.updateCalibrationScreen(calibrationStarted, currentDistance);
}

// ============================================================================
// AKTUALIZACJA EKRANU POMIARU DYSTANSU
// ============================================================================

void UIManager::updateDistanceMeasureScreen() {
    float distance = 0.0;
    if (distanceMeasureStarted) {
        distance = encoderManager.getDistanceCm() - distanceMeasureStart;
    }
    displayManager.updateDistanceMeasureScreen(distanceMeasureStarted, distance);
}

// ============================================================================
// OBSŁUGA PRZYCISKU START/PAUZA
// ============================================================================

void UIManager::handleStartPause() {
    switch (currentState) {
        case STATE_IDLE:
        case STATE_PAUSED:
            // Sprawdzenie czy wybrano wzorzec
            if (patternManager.isPatternActive()) {
                setState(STATE_PAINTING);
                dataLogger.logEvent("START_PAINTING", patternManager.getPatternName());
                Serial.println("UIManager: START malowania");
            } else {
                displayManager.showMessage("BLAD", "Wybierz wzorzec!", COLOR_INACTIVE);
            }
            break;
            
        case STATE_PAINTING:
            // Zapisz raport przed pauzą
            dataLogger.logWork(encoderManager.getDistanceCm(), totalPaintedArea_m2, encoderManager.getSpeed_kmh(), patternManager.getPatternName());
            dataLogger.logEvent("PAUSE_PAINTING", "Pauza operatora");
            
            setState(STATE_PAUSED);
            Serial.println("UIManager: PAUZA");
            break;
            
        case STATE_CALIBRATION:
            if (!calibrationStarted) {
                // Start kalibracji
                encoderManager.startCalibration();
                calibrationStarted = true;
                calibrationStartDistance = 0.0;
                Serial.println("UIManager: Start kalibracji");
            } else {
                // Zakończenie kalibracji
                encoderManager.finishCalibration(CALIBRATION_DISTANCE_CM);
                displayManager.showMessage("SUKCES", "Kalibracja zakonczona", COLOR_ACTIVE);
                setState(STATE_IDLE);
            }
            break;
            
        case STATE_DISTANCE_MEASURE:
            if (!distanceMeasureStarted) {
                // Start pomiaru
                encoderManager.resetDistance();
                distanceMeasureStart = 0.0;
                distanceMeasureStarted = true;
                Serial.println("UIManager: Start pomiaru dystansu");
            } else {
                // Zatrzymanie pomiaru (pozostaje na ekranie)
                Serial.printf("UIManager: Pomiar: %.2f m\n", 
                    (encoderManager.getDistanceCm() - distanceMeasureStart) / 100.0);
            }
            break;
            
        default:
            break;
    }
}

// ============================================================================
// OBSŁUGA PRZYCISKU STOP
// ============================================================================

void UIManager::handleStop(PressType pressType) {
    if (pressType == PRESS_LONG) {
        // Długie naciśnięcie (1s) - wejście do menu z ekranu pracy
        if (currentState == STATE_IDLE || currentState == STATE_PAUSED) {
            setState(STATE_MENU);
            Serial.println("UIManager: Wejście do MENU");
        }
    } else if (pressType == PRESS_VERY_LONG) {
        // Bardzo długie naciśnięcie (2s) - wyjście z funkcji
        if (currentState == STATE_CALIBRATION || 
            currentState == STATE_DISTANCE_MEASURE || 
            currentState == STATE_MENU) {
            setState(STATE_IDLE);
            Serial.println("UIManager: Wyjście do ekranu głównego");
        }
    } else if (pressType == PRESS_SHORT) {
        // Krótkie naciśnięcie - zatrzymanie malowania
        if (currentState == STATE_PAINTING) {
            setState(STATE_PAUSED);
            Serial.println("UIManager: STOP malowania");
        }
    }
}

// ============================================================================
// OBSŁUGA SELEKTORA
// ============================================================================

void UIManager::handleSelector(PressType pressType) {
    if (currentState == STATE_MENU) {
        if (pressType == PRESS_SHORT) {
            // Krótkie - przełączanie pozycji menu
            displayManager.nextMenuItem();
        } else if (pressType == PRESS_LONG) {
            // Długie - wybór pozycji menu
            MenuPosition selected = displayManager.getSelectedMenuItem();
            
            switch (selected) {
                case MENU_CALIBRATION:
                    setState(STATE_CALIBRATION);
                    Serial.println("UIManager: Wybrano KALIBRACJĘ");
                    break;
                case MENU_DISTANCE_MEASURE:
                    setState(STATE_DISTANCE_MEASURE);
                    Serial.println("UIManager: Wybrano POMIAR DYSTANSU");
                    break;
                case MENU_SETTINGS:
                    displayManager.showMessage("INFO", "Ustawienia - w przygotowaniu");
                    break;
                case MENU_BACK:
                    setState(STATE_IDLE);
                    Serial.println("UIManager: Powrót do ekranu głównego");
                    break;
                default:
                    break;
            }
        }
    }
}

// ============================================================================
// OBSŁUGA PRZYCISKÓW WZORCÓW
// ============================================================================

void UIManager::handlePatternButton(PatternType pattern) {
    if (pattern != PATTERN_NONE) {
        // Zmiana wzorca "w locie"
        patternManager.selectPattern(pattern);
        
        Serial.printf("UIManager: Wybrano wzorzec %s\n", patternManager.getPatternName());
        
        // Jeśli jesteśmy w stanie IDLE, przejdź do PAUSED (gotowość)
        if (currentState == STATE_IDLE) {
            setState(STATE_PAUSED);
        }
    }
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

UIManager uiManager;

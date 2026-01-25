#include "button_handler.h"

// ============================================================================
// DEFINICJE STATYCZNE
// ============================================================================

const uint8_t ButtonHandler::patternPins[15] = {
    BTN_P1A, BTN_P1B, BTN_P1C, BTN_P1D, BTN_P1E,
    BTN_P2A, BTN_P2B,
    BTN_P3A, BTN_P3B,
    BTN_P4,
    BTN_P6,
    BTN_P7A, BTN_P7B, BTN_P7C, BTN_P7D
};

// ============================================================================
// KONSTRUKTOR
// ============================================================================

ButtonHandler::ButtonHandler() {
    btnStartPause = {BTN_START_PAUSE, HIGH, 0, false, PRESS_NONE};
    btnStop = {BTN_STOP, HIGH, 0, false, PRESS_NONE};
    btnSelector = {ENCODER_SW, HIGH, 0, false, PRESS_NONE};
    
    for (int i = 0; i < 15; i++) {
        btnPatterns[i] = {patternPins[i], HIGH, 0, false, PRESS_NONE};
    }
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

void ButtonHandler::begin() {
    // Konfiguracja przycisków sterowania
    pinMode(BTN_START_PAUSE, INPUT_PULLUP);
    pinMode(BTN_STOP, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    
    // Konfiguracja przycisków wzorców
    for (int i = 0; i < 15; i++) {
        pinMode(patternPins[i], INPUT_PULLUP);
    }
    
    Serial.println("ButtonHandler: Inicjalizacja zakończona");
}

// ============================================================================
// AKTUALIZACJA
// ============================================================================

void ButtonHandler::update() {
    updateButton(btnStartPause);
    updateButton(btnStop);
    updateButton(btnSelector);
    
    for (int i = 0; i < 15; i++) {
        updateButton(btnPatterns[i]);
    }
}

// ============================================================================
// OBSŁUGA POJEDYNCZEGO PRZYCISKU
// ============================================================================

void ButtonHandler::updateButton(Button& btn) {
    bool currentState = digitalRead(btn.pin);
    unsigned long currentTime = millis();
    
    // Detekcja naciśnięcia (zbocze opadające - przycisk z pull-up)
    if (currentState == LOW && btn.lastState == HIGH) {
        // Debouncing
        delay(DEBOUNCE_MS);
        currentState = digitalRead(btn.pin);
        
        if (currentState == LOW) {
            btn.pressed = true;
            btn.pressStartTime = currentTime;
            btn.pressType = PRESS_NONE;
        }
    }
    
    // Detekcja zwolnienia
    if (currentState == HIGH && btn.lastState == LOW) {
        if (btn.pressed) {
            unsigned long pressDuration = currentTime - btn.pressStartTime;
            
            if (pressDuration >= VERY_LONG_PRESS_MS) {
                btn.pressType = PRESS_VERY_LONG;
            } else if (pressDuration >= LONG_PRESS_MS) {
                btn.pressType = PRESS_LONG;
            } else {
                btn.pressType = PRESS_SHORT;
            }
            
            btn.pressed = false;
        }
    }
    
    btn.lastState = currentState;
}

// ============================================================================
// POBIERANIE STANU PRZYCISKÓW STEROWANIA
// ============================================================================

PressType ButtonHandler::getStartPausePress() {
    return getPress(btnStartPause);
}

PressType ButtonHandler::getStopPress() {
    return getPress(btnStop);
}

PressType ButtonHandler::getSelectorPress() {
    return getPress(btnSelector);
}

// ============================================================================
// POBIERANIE I RESETOWANIE STANU PRZYCISKU
// ============================================================================

PressType ButtonHandler::getPress(Button& btn) {
    PressType type = btn.pressType;
    btn.pressType = PRESS_NONE;
    return type;
}

// ============================================================================
// SPRAWDZANIE PRZYCISKÓW WZORCÓW
// ============================================================================

PatternType ButtonHandler::getPatternButtonPressed() {
    for (int i = 0; i < 15; i++) {
        if (btnPatterns[i].pressType == PRESS_SHORT) {
            btnPatterns[i].pressType = PRESS_NONE;
            return (PatternType)i;
        }
    }
    return PATTERN_NONE;
}

// ============================================================================
// RESETOWANIE STANU
// ============================================================================

void ButtonHandler::reset() {
    btnStartPause.pressType = PRESS_NONE;
    btnStop.pressType = PRESS_NONE;
    btnSelector.pressType = PRESS_NONE;
    
    for (int i = 0; i < 15; i++) {
        btnPatterns[i].pressType = PRESS_NONE;
    }
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

ButtonHandler buttonHandler;

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include "config.h"
#include "patterns.h"

// ============================================================================
// TYPY NACIŚNIĘĆ
// ============================================================================

enum PressType {
    PRESS_NONE = 0,
    PRESS_SHORT,
    PRESS_LONG,
    PRESS_VERY_LONG
};

// ============================================================================
// STRUKTURA PRZYCISKU
// ============================================================================

struct Button {
    uint8_t pin;
    bool lastState;
    unsigned long pressStartTime;
    bool pressed;
    PressType pressType;
};

// ============================================================================
// KLASA OBSŁUGI PRZYCISKÓW
// ============================================================================

class ButtonHandler {
public:
    ButtonHandler();
    
    // Inicjalizacja
    void begin();
    
    // Aktualizacja (wywołuj w loop)
    void update();
    
    // Sprawdzanie stanu przycisków sterowania
    PressType getStartPausePress();
    PressType getStopPress();
    PressType getSelectorPress();
    PressType getStartGapPress();     // Nowy przycisk Start Gap
    PressType getReverseP3Press();    // Nowy przycisk odwracania P-3
    
    // Sprawdzanie przycisków wzorców
    PatternType getPatternButtonPressed();
    
    // Resetowanie stanu przycisków
    void reset();
    
private:
    // Przyciski sterowania
    Button btnStartPause;
    Button btnStop;
    Button btnSelector;
    Button btnStartGap;      // Przycisk Start Gap
    Button btnReverseP3;     // Przycisk odwracania P-3
    
    // Przyciski wzorców (15 przycisków)
    Button btnPatterns[15];
    
    // Tablica pinów wzorców
    static const uint8_t patternPins[15];
    
    // Obsługa pojedynczego przycisku
    void updateButton(Button& btn);
    PressType getPress(Button& btn);
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern ButtonHandler buttonHandler;

#endif // BUTTON_HANDLER_H

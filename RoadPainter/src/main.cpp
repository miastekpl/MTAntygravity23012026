/*
 * SYSTEM MALOWANIA PASÓW DROGOWYCH
 * Wersja: 1.0.0
 * Data: 2026-01-25
 * 
 * Platforma: ESP32-S3 N16R8
 * Wyświetlacz: ILI9341 320x240
 * Enkoder: KY-040
 * 
 * Funkcje:
 * - 15 wzorców malowania zgodnych z polskimi normami
 * - Kalibracja enkodera (10m)
 * - Pomiar dystansu z dokładnością 1cm
 * - Zmiana wzorców "w locie"
 * - Sterowanie 6 pistoletami malarskimi
 * - Licznik wymalowanych m²
 * 
 * (c) 2026 - System produkcyjny
 */

#include <Arduino.h>
#include "config.h"
#include "patterns.h"
#include "encoder.h"
#include "relay_controller.h"
#include "button_handler.h"
#include "display.h"
#include "ui_manager.h"

// ============================================================================
// SETUP
// ============================================================================

void setup() {
    // Inicjalizacja komunikacji szeregowej
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n========================================");
    Serial.println("SYSTEM MALOWANIA PASOW DROGOWYCH v1.0.0");
    Serial.println("========================================\n");
    
    // Inicjalizacja komponentów
    Serial.println("Inicjalizacja komponentow...");
    
    // 1. Wyświetlacz (pierwszy - komunikaty wizualne)
    displayManager.begin();
    
    // 2. Enkoder
    encoderManager.begin();
    
    // 3. Przekaźniki
    relayController.begin();
    
    // 4. Przyciski
    buttonHandler.begin();
    
    // 5. Wzorce
    patternManager.begin();
    
    // 6. UI Manager
    uiManager.begin();
    
    Serial.println("\nSystem gotowy do pracy!");
    Serial.println("========================================\n");
    
    // Sprawdzenie kalibracji
    if (!encoderManager.isCalibrated()) {
        displayManager.showMessage("UWAGA", "Wymagana kalibracja!", COLOR_WARNING);
    }
    
    // Test przekaźników (opcjonalny - zakomentuj jeśli niepotrzebny)
    // relayController.testSequence();
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
    // 1. Aktualizacja enkodera (pomiar dystansu i prędkości)
    encoderManager.update();
    
    // 2. Aktualizacja przycisków
    buttonHandler.update();
    
    // 3. Obsługa przycisków sterowania
    PressType startPausePress = buttonHandler.getStartPausePress();
    if (startPausePress == PRESS_SHORT) {
        uiManager.handleStartPause();
    }
    
    PressType stopPress = buttonHandler.getStopPress();
    if (stopPress != PRESS_NONE) {
        uiManager.handleStop(stopPress);
    }
    
    PressType selectorPress = buttonHandler.getSelectorPress();
    if (selectorPress != PRESS_NONE) {
        uiManager.handleSelector(selectorPress);
    }
    
    // Obsługa przycisku Start Gap
    PressType startGapPress = buttonHandler.getStartGapPress();
    if (startGapPress == PRESS_SHORT) {
        patternManager.setStartGap(!patternManager.isStartGapActive());
    }
    
    // Obsługa przycisku odwracania P-3
    PressType reverseP3Press = buttonHandler.getReverseP3Press();
    if (reverseP3Press == PRESS_SHORT) {
        patternManager.toggleReversed();
    }
    
    // 4. Obsługa przycisków wzorców
    PatternType selectedPattern = buttonHandler.getPatternButtonPressed();
    if (selectedPattern != PATTERN_NONE) {
        uiManager.handlePatternButton(selectedPattern);
    }
    
    // 5. Aktualizacja UI (ekrany, logika malowania)
    uiManager.update();
    
    // 6. Aktualizacja wyświetlacza
    displayManager.update();
    
    // Małe opóźnienie dla stabilności
    delay(10);
}

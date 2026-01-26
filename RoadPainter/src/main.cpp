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

#include <esp_task_wdt.h>
#include "web_server.h"
#include "data_logger.h"

// ============================================================================
// SETUP
// ============================================================================

void setup() {
    // Inicjalizacja WDT (Watchdog Timer) - zabezpieczenie przed zawieszeniem
    esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
    esp_task_wdt_add(NULL);  // Dodaj bieżący wątek (loop) do WDT
    
    // Inicjalizacja komunikacji szeregowej
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n========================================");
    Serial.println("SYSTEM MALOWANIA PASOW DROGOWYCH v1.6.0 (FINAL)");
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
    
    // 7. Serwer WWW
    webServer.begin();
    
    // 8. Logger Danych (SD)
    dataLogger.begin();
    
    Serial.println("\nSystem gotowy do pracy!");
    Serial.println("========================================\n");
    
    // Sprawdzenie kalibracji
    if (!encoderManager.isCalibrated()) {
        displayManager.showMessage("UWAGA", "Wymagana kalibracja!", COLOR_WARNING);
    }
    
    // Konfiguracja pinu E-STOP
    pinMode(BTN_EMERGENCY_STOP, INPUT_PULLUP);
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
    // 0. Emergency Stop - Najwyższy priorytet
    if (digitalRead(BTN_EMERGENCY_STOP) == LOW) {
        relayController.allOff();
        displayManager.showMessage("E-STOP", "ZATRZYMANIE AWARYJNE!", COLOR_INACTIVE);
        Serial.println("!!! E-STOP ACTIVATED !!!");
        
        // Pętla blokująca z obsługą WDT, wyjście tylko po resecie zasilania
        while (true) {
            esp_task_wdt_reset();
            relayController.allOff(); // Dla pewności ciągłe wymuszanie OFF
            delay(100);
        }
    }

    // Resetuj Watchdog Timer
    esp_task_wdt_reset();
    
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
    
    // 7. Aktualizacja serwera WWW
    webServer.update();
    
    // Małe opóźnienie dla stabilności (NIE BLOKUJE PĘTLI)
    // Usunięcie delay() na rzecz millis() w przyszłości
    delay(5); 
}

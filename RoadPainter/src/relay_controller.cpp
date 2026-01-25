#include "relay_controller.h"

// ============================================================================
// DEFINICJE STATYCZNE
// ============================================================================

const uint8_t RelayController::relayPins[6] = {
    RELAY_1, RELAY_2, RELAY_3, RELAY_4, RELAY_5, RELAY_6
};

// ============================================================================
// KONSTRUKTOR
// ============================================================================

RelayController::RelayController()
    : currentMask(0)
{
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

void RelayController::begin() {
    // Konfiguracja pinów jako wyjścia
    for (int i = 0; i < 6; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);  // Przekaźniki wyłączone
    }
    
    currentMask = 0;
    
    Serial.println("RelayController: Inicjalizacja zakończona - wszystkie pistolety wyłączone");
}

// ============================================================================
// USTAWIANIE MASKI PISTOLETÓW
// ============================================================================

void RelayController::setGunMask(uint8_t mask) {
    currentMask = mask & 0b00111111;  // Tylko 6 bitów
    updateRelays();
}

// ============================================================================
// WŁĄCZANIE/WYŁĄCZANIE POJEDYNCZEGO PISTOLETU
// ============================================================================

void RelayController::setGun(uint8_t gunNumber, bool state) {
    if (gunNumber < 1 || gunNumber > 6) {
        return;
    }
    
    uint8_t bitPosition = gunNumber - 1;
    
    if (state) {
        currentMask |= (1 << bitPosition);   // Ustaw bit
    } else {
        currentMask &= ~(1 << bitPosition);  // Wyczyść bit
    }
    
    updateRelays();
}

// ============================================================================
// WYŁĄCZANIE WSZYSTKICH PISTOLETÓW
// ============================================================================

void RelayController::allOff() {
    currentMask = 0;
    updateRelays();
    Serial.println("RelayController: Wszystkie pistolety wyłączone");
}

// ============================================================================
// AKTUALIZACJA STANU PRZEKAŹNIKÓW
// ============================================================================

void RelayController::updateRelays() {
    for (int i = 0; i < 6; i++) {
        bool state = (currentMask >> i) & 0x01;
        digitalWrite(relayPins[i], state ? HIGH : LOW);
    }
}

// ============================================================================
// TEST SEKWENCYJNY
// ============================================================================

void RelayController::testSequence() {
    Serial.println("RelayController: Test sekwencyjny pistoletów...");
    
    allOff();
    delay(500);
    
    for (int i = 1; i <= 6; i++) {
        Serial.printf("  Pistolet %d ON\n", i);
        setGun(i, true);
        delay(500);
        setGun(i, false);
        delay(200);
    }
    
    Serial.println("  Wszystkie ON");
    setGunMask(0b00111111);
    delay(1000);
    
    allOff();
    Serial.println("RelayController: Test zakończony");
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

RelayController relayController;

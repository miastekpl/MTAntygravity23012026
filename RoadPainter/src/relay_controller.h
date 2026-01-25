#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// KLASA STEROWANIA PRZEKAŹNIKAMI
// ============================================================================

class RelayController {
public:
    RelayController();
    
    // Inicjalizacja
    void begin();
    
    // Ustawianie maski pistoletów (bity 0-5 odpowiadają pistoletom 1-6)
    void setGunMask(uint8_t mask);
    
    // Włączanie/wyłączanie pojedynczego pistoletu
    void setGun(uint8_t gunNumber, bool state);
    
    // Wyłączanie wszystkich pistoletów
    void allOff();
    
    // Pobieranie aktualnej maski
    uint8_t getCurrentMask() const { return currentMask; }
    
    // Test wszystkich pistoletów (sekwencyjnie)
    void testSequence();
    
private:
    uint8_t currentMask;
    
    // Tablica pinów przekaźników
    static const uint8_t relayPins[6];
    
    // Aktualizacja stanu przekaźników
    void updateRelays();
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern RelayController relayController;

#endif // RELAY_CONTROLLER_H

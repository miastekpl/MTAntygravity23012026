#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// KLASA OBSŁUGI ENKODERA I POMIARÓW
// ============================================================================

class EncoderManager {
public:
    EncoderManager();
    
    // Inicjalizacja
    void begin();
    
    // Obsługa przerwań (muszą być static)
    static void IRAM_ATTR handleEncoderISR();
    
    // Aktualizacja (wywołuj w loop)
    void update();
    
    // Pobieranie danych
    float getDistanceCm() const { return totalDistance_cm; }
    float getSpeed_kmh() const { return speed_kmh; }
    float getDeltaDistance_cm();  // Zwraca przyrost od ostatniego wywołania
    
    // Resetowanie dystansu
    void resetDistance();
    
    // Kalibracja
    void startCalibration();
    void finishCalibration(float actualDistance_cm);
    bool isCalibrated() const { return calibrated; }
    float getCalibrationFactor() const { return calibrationFactor; }
    
    // Zapisywanie/wczytywanie kalibracji z EEPROM
    void saveCalibration();
    void loadCalibration();
    
private:
    // Licznik impulsów (volatile - modyfikowany w ISR)
    static volatile long pulseCount;
    static volatile long lastPulseCount;
    
    // Dane pomiarowe
    float totalDistance_cm;
    float lastDistance_cm;
    float speed_kmh;
    
    // Kalibracja
    bool calibrated;
    float calibrationFactor;  // Współczynnik korekcyjny
    
    // Pomiar prędkości
    unsigned long lastSpeedCalc_ms;
    long lastPulseCountForSpeed;
    
    // Przeliczanie impulsów na dystans
    float pulsesToDistance(long pulses) const;
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern EncoderManager encoderManager;

#endif // ENCODER_H

#include "encoder.h"
#include <EEPROM.h>

// ============================================================================
// ZMIENNE STATYCZNE
// ============================================================================

volatile long EncoderManager::pulseCount = 0;
volatile long EncoderManager::lastPulseCount = 0;

// Wskaźnik do instancji (dla ISR)
static EncoderManager* encoderInstance = nullptr;

// ============================================================================
// KONSTRUKTOR
// ============================================================================

EncoderManager::EncoderManager()
    : totalDistance_cm(0.0)
    , lastDistance_cm(0.0)
    , speed_kmh(0.0)
    , calibrated(false)
    , calibrationFactor(1.0)
    , lastSpeedCalc_ms(0)
    , lastPulseCountForSpeed(0)
{
    encoderInstance = this;
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

void EncoderManager::begin() {
    // Konfiguracja pinów enkodera
    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    
    // Przerwania na zbocze opadające CLK
    attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), handleEncoderISR, FALLING);
    
    // Inicjalizacja EEPROM
    EEPROM.begin(EEPROM_SIZE);
    
    // Wczytanie kalibracji
    loadCalibration();
    
    // Inicjalizacja liczników
    pulseCount = 0;
    lastPulseCount = 0;
    lastSpeedCalc_ms = millis();
    lastPulseCountForSpeed = 0;
    
    Serial.println("EncoderManager: Inicjalizacja zakończona");
    if (calibrated) {
        Serial.printf("Kalibracja wczytana: %.4f\n", calibrationFactor);
    } else {
        Serial.println("UWAGA: Enkoder wymaga kalibracji!");
    }
}

// ============================================================================
// OBSŁUGA PRZERWANIA ENKODERA
// ============================================================================

void IRAM_ATTR EncoderManager::handleEncoderISR() {
    // Odczyt stanu DT dla określenia kierunku
    int dtState = digitalRead(ENCODER_DT);
    
    if (dtState == LOW) {
        pulseCount++;  // Obrót w przód
    } else {
        pulseCount--;  // Obrót w tył (jeśli konieczne)
    }
}

// ============================================================================
// AKTUALIZACJA
// ============================================================================

void EncoderManager::update() {
    unsigned long currentTime = millis();
    
    // Aktualizacja dystansu
    long currentPulses = pulseCount;
    totalDistance_cm = pulsesToDistance(currentPulses);
    
    // Obliczanie prędkości co SPEED_CALC_INTERVAL_MS
    if (currentTime - lastSpeedCalc_ms >= SPEED_CALC_INTERVAL_MS) {
        long pulseDelta = currentPulses - lastPulseCountForSpeed;
        float distanceDelta_cm = pulsesToDistance(pulseDelta);
        float timeDelta_s = (currentTime - lastSpeedCalc_ms) / 1000.0;
        
        if (timeDelta_s > 0) {
            // Prędkość = dystans / czas
            // cm/s -> km/h: × 0.036
            speed_kmh = (distanceDelta_cm / timeDelta_s) * 0.036;
            
            // Zabezpieczenie przed nierealistycznymi wartościami
            if (speed_kmh > MAX_SPEED_KMH) {
                speed_kmh = MAX_SPEED_KMH;
            }
            if (speed_kmh < 0) {
                speed_kmh = 0;
            }
        }
        
        lastSpeedCalc_ms = currentTime;
        lastPulseCountForSpeed = currentPulses;
    }
}

// ============================================================================
// POBIERANIE PRZYROSTU DYSTANSU
// ============================================================================

float EncoderManager::getDeltaDistance_cm() {
    float delta = totalDistance_cm - lastDistance_cm;
    lastDistance_cm = totalDistance_cm;
    return delta;
}

// ============================================================================
// RESETOWANIE DYSTANSU
// ============================================================================

void EncoderManager::resetDistance() {
    pulseCount = 0;
    lastPulseCount = 0;
    totalDistance_cm = 0.0;
    lastDistance_cm = 0.0;
    speed_kmh = 0.0;
    lastPulseCountForSpeed = 0;
    
    Serial.println("EncoderManager: Dystans zresetowany");
}

// ============================================================================
// KALIBRACJA
// ============================================================================

void EncoderManager::startCalibration() {
    resetDistance();
    Serial.println("EncoderManager: Rozpoczęto kalibrację - przejedź dokładnie 10m");
}

void EncoderManager::finishCalibration(float actualDistance_cm) {
    long pulses = pulseCount;
    
    if (pulses == 0) {
        Serial.println("EncoderManager: BŁĄD - brak impulsów podczas kalibracji!");
        return;
    }
    
    // Obliczenie współczynnika kalibracji
    float measuredDistance_cm = pulsesToDistance(pulses);
    calibrationFactor = actualDistance_cm / measuredDistance_cm;
    calibrated = true;
    
    // Zapisanie do EEPROM
    saveCalibration();
    
    Serial.printf("EncoderManager: Kalibracja zakończona\n");
    Serial.printf("  Impulsy: %ld\n", pulses);
    Serial.printf("  Dystans rzeczywisty: %.2f cm\n", actualDistance_cm);
    Serial.printf("  Dystans zmierzony: %.2f cm\n", measuredDistance_cm);
    Serial.printf("  Współczynnik kalibracji: %.4f\n", calibrationFactor);
    
    // Reset po kalibracji
    resetDistance();
}

// ============================================================================
// PRZELICZANIE IMPULSÓW NA DYSTANS
// ============================================================================

float EncoderManager::pulsesToDistance(long pulses) const {
    // Dystans = (impulsy / impulsy_na_obrót) × obwód_koła × współczynnik_kalibracji
    float distance = (float)pulses / ENCODER_PULSES_PER_REV * WHEEL_CIRCUMFERENCE_MM / 10.0;
    return distance * calibrationFactor;
}

// ============================================================================
// ZAPISYWANIE KALIBRACJI DO EEPROM
// ============================================================================

void EncoderManager::saveCalibration() {
    EEPROM.put(EEPROM_ADDR_CALIBRATION, calibrationFactor);
    EEPROM.write(EEPROM_ADDR_INITIALIZED, EEPROM_MAGIC_BYTE);
    EEPROM.commit();
    
    Serial.println("EncoderManager: Kalibracja zapisana do EEPROM");
}

// ============================================================================
// WCZYTYWANIE KALIBRACJI Z EEPROM
// ============================================================================

void EncoderManager::loadCalibration() {
    byte magicByte = EEPROM.read(EEPROM_ADDR_INITIALIZED);
    
    if (magicByte == EEPROM_MAGIC_BYTE) {
        EEPROM.get(EEPROM_ADDR_CALIBRATION, calibrationFactor);
        
        // Walidacja
        if (calibrationFactor > 0.5 && calibrationFactor < 2.0) {
            calibrated = true;
            Serial.println("EncoderManager: Kalibracja wczytana z EEPROM");
        } else {
            calibrationFactor = 1.0;
            calibrated = false;
            Serial.println("EncoderManager: Nieprawidłowa kalibracja w EEPROM");
        }
    } else {
        calibrationFactor = 1.0;
        calibrated = false;
        Serial.println("EncoderManager: Brak kalibracji w EEPROM");
    }
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

EncoderManager encoderManager;

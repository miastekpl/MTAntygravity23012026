#include "encoder.h"
#include <Preferences.h>

// ============================================================================
// ZMIENNE STATYCZNE
// ============================================================================

volatile long EncoderManager::pulseCount = 0;
volatile long EncoderManager::lastPulseCount = 0;
portMUX_TYPE EncoderManager::timerMux = portMUX_INITIALIZER_UNLOCKED;

// Wskaźnik do instancji (dla ISR)
static EncoderManager* encoderInstance = nullptr;
static Preferences preferences;

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
    
    // Wczytanie kalibracji z NVS (Preferences - wbudowany wear leveling)
    preferences.begin("roadpainter", false);
    loadCalibration();
    
    // Inicjalizacja liczników
    portENTER_CRITICAL(&timerMux);
    pulseCount = 0;
    portEXIT_CRITICAL(&timerMux);
    
    lastPulseCount = 0;
    lastSpeedCalc_ms = millis();
    lastPulseCountForSpeed = 0;
    
    Serial.println("EncoderManager: Inicjalizacja zakończona (Safe ISR + NVS)");
    if (calibrated) {
        Serial.printf("Kalibracja wczytana: %.4f\n", calibrationFactor);
    } else {
        Serial.println("UWAGA: Enkoder wymaga kalibracji!");
    }
}

// ============================================================================
// OBSŁUGA PRZERWANIA ENKODERA (SAFE ISR)
// ============================================================================

void IRAM_ATTR EncoderManager::handleEncoderISR() {
    // Odczyt rejestrów GPIO (szybsze niż digitalRead)
    // ESP32-S3 GPIO 0-31 są w rejestrze GPIO_IN_REG
    // ENCODER_DT to GPIO 5, więc bit 5
    
    uint32_t gpio_status = REG_READ(GPIO_IN_REG);
    bool dtState = (gpio_status >> ENCODER_DT) & 0x01;
    
    portENTER_CRITICAL_ISR(&timerMux);
    if (!dtState) {
        pulseCount++;  // Obrót w przód
    } else {
        pulseCount--;  // Obrót w tył
    }
    portEXIT_CRITICAL_ISR(&timerMux);
}

// ============================================================================
// AKTUALIZACJA
// ============================================================================

void EncoderManager::update() {
    unsigned long currentTime = millis();
    
    // Atomowy odczyt licznika
    long currentPulses;
    portENTER_CRITICAL(&timerMux);
    currentPulses = pulseCount;
    portEXIT_CRITICAL(&timerMux);
    
    // Aktualizacja dystansu
    totalDistance_cm = pulsesToDistance(currentPulses);
    
    // Obliczanie prędkości co SPEED_CALC_INTERVAL_MS
    if (currentTime - lastSpeedCalc_ms >= SPEED_CALC_INTERVAL_MS) {
        long pulseDelta = currentPulses - lastPulseCountForSpeed;
        float distanceDelta_cm = pulsesToDistance(pulseDelta);
        float timeDelta_s = (currentTime - lastSpeedCalc_ms) / 1000.0;
        
        if (timeDelta_s > 0) {
            // Prędkość = dystans / czas
            // cm/s -> km/h: × 0.036
            
            // Prosty filtr dolnoprzepustowy dla prędkości (waga 0.7 nowy, 0.3 stary)
            float rawSpeed = (distanceDelta_cm / timeDelta_s) * 0.036;
            speed_kmh = (rawSpeed * 0.7) + (speed_kmh * 0.3);
            
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
    // Atomowy dostęp do zmiennych
    // W tym przypadku totalDistance_cm jest obliczane tylko w update(), który jest w głównym wątku,
    // więc race condition z ISR nie występuje bezpośrednio, ale dobra praktyka zachowana.
    
    float delta = totalDistance_cm - lastDistance_cm;
    lastDistance_cm = totalDistance_cm;
    return delta;
}

// ============================================================================
// RESETOWANIE DYSTANSU
// ============================================================================

void EncoderManager::resetDistance() {
    portENTER_CRITICAL(&timerMux);
    pulseCount = 0;
    portEXIT_CRITICAL(&timerMux);
    
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
    long pulses;
    portENTER_CRITICAL(&timerMux);
    pulses = pulseCount;
    portEXIT_CRITICAL(&timerMux);
    
    if (pulses == 0) {
        Serial.println("EncoderManager: BŁĄD - brak impulsów podczas kalibracji!");
        return;
    }
    
    // Obliczenie współczynnika kalibracji
    // Tymczasowo ustawiamy factor na 1.0 żeby obliczyć surowy dystans
    float oldFactor = calibrationFactor;
    calibrationFactor = 1.0;
    float measuredRawDistance_cm = pulsesToDistance(pulses);
    calibrationFactor = oldFactor;
    
    // Nowy factor = cel / pomiar_surowy
    calibrationFactor = actualDistance_cm / measuredRawDistance_cm;
    calibrated = true;
    
    // Zapisanie do Preferences (NVS)
    saveCalibration();
    
    Serial.printf("EncoderManager: Kalibracja zakończona\n");
    Serial.printf("  Impulsy: %ld\n", pulses);
    Serial.printf("  Dystans surowy: %.2f cm\n", measuredRawDistance_cm);
    Serial.printf("  Nowy współczynnik: %.4f\n", calibrationFactor);
    
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
// ZAPISYWANIE KALIBRACJI DO NVS (Preferences)
// ============================================================================

void EncoderManager::saveCalibration() {
    preferences.putFloat("calFactor", calibrationFactor);
    preferences.putBool("calInit", true);
    Serial.println("EncoderManager: Kalibracja zapisana do NVS");
}

// ============================================================================
// WCZYTYWANIE KALIBRACJI Z NVS (Preferences)
// ============================================================================

void EncoderManager::loadCalibration() {
    bool initialized = preferences.getBool("calInit", false);
    
    if (initialized) {
        calibrationFactor = preferences.getFloat("calFactor", 1.0);
        
        // Walidacja
        if (calibrationFactor > 0.5 && calibrationFactor < 2.0) {
            calibrated = true;
            Serial.println("EncoderManager: Kalibracja wczytana z NVS");
        } else {
            calibrationFactor = 1.0;
            calibrated = false;
            Serial.println("EncoderManager: Nieprawidłowa kalibracja w NVS");
        }
    } else {
        calibrationFactor = 1.0;
        calibrated = false;
        Serial.println("EncoderManager: Brak kalibracji w NVS");
    }
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

EncoderManager encoderManager;

#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "config.h"

// ============================================================================
// KLASA LOGGERA DANYCH
// ============================================================================

class DataLogger {
public:
    DataLogger();
    
    // Inicjalizacja
    bool begin();
    
    // Logowanie danych pracy (co minutę lub przy pauzie)
    void logWork(float totalDistance, float totalArea, float currentSpeed, const char* patternName);
    
    // Logowanie zdarzeń (Start, Stop, Błąd)
    void logEvent(const char* eventType, const char* details);
    
    // Sprawdzenie czy karta jest dostępna
    bool isAvailable() const { return initialized; }
    
private:
    bool initialized;
    String currentFilename;
    
    // Generowanie nowej nazwy pliku (np. LOG_001.CSV)
    void createNewLogFile();
    
    // Helper do zapisu linii
    void writeLine(const String& line);
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern DataLogger dataLogger;

#endif // DATA_LOGGER_H

#include "data_logger.h"

// ============================================================================
// KONSTRUKTOR
// ============================================================================

DataLogger::DataLogger() 
    : initialized(false)
{
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

bool DataLogger::begin() {
    Serial.print("DataLogger: Inicjalizacja karty SD na pinie ");
    Serial.println(SD_CS_PIN);
    
    // Inicjalizacja SD
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("DataLogger: BŁĄD - Nie można zainicjować karty SD!");
        initialized = false;
        return false;
    }
    
    Serial.println("DataLogger: Karta SD wykryta");
    initialized = true;
    
    // Utworzenie pliku logów
    createNewLogFile();
    
    logEvent("SYSTEM_START", "Uruchomienie systemu v1.4.0");
    return true;
}

// ============================================================================
// TWORZENIE PLIKU LOGÓW
// ============================================================================

void DataLogger::createNewLogFile() {
    // Szukanie wolnej nazwy pliku LOG_XXX.CSV
    int fileIndex = 1;
    do {
        currentFilename = "/LOG_" + String(fileIndex) + ".CSV";
        fileIndex++;
    } while (SD.exists(currentFilename));
    
    Serial.print("DataLogger: Utworzono plik ");
    Serial.println(currentFilename);
    
    // Zapis nagłówka CSV
    File file = SD.open(currentFilename, FILE_WRITE);
    if (file) {
        file.println("TIMESTAMP_MS;EVENT_TYPE;PATTERN;DISTANCE_M;AREA_M2;SPEED_KMH;DETAILS");
        file.close();
    }
}

// ============================================================================
// LOGOWANIE PRACY
// ============================================================================

void DataLogger::logWork(float totalDistance, float totalArea, float currentSpeed, const char* patternName) {
    if (!initialized) return;
    
    String line = String(millis()) + ";WORK;" + String(patternName) + ";" +
                  String(totalDistance/100.0, 2) + ";" +
                  String(totalArea, 3) + ";" +
                  String(currentSpeed, 1) + ";-";
                  
    writeLine(line);
}

// ============================================================================
// LOGOWANIE ZDARZEŃ
// ============================================================================

void DataLogger::logEvent(const char* eventType, const char* details) {
    if (!initialized) return;
    
    String line = String(millis()) + ";" + String(eventType) + ";-;-;-;-;" + String(details);
    writeLine(line);
    
    Serial.printf("Logger: %s - %s\n", eventType, details);
}

// ============================================================================
// ZAPIS LINII
// ============================================================================

void DataLogger::writeLine(const String& line) {
    if (!initialized) return;
    
    File file = SD.open(currentFilename, FILE_APPEND);
    if (file) {
        file.println(line);
        file.close();
    } else {
        Serial.println("DataLogger: Błąd zapisu do pliku!");
        // Próba ponownej inicjalizacji przy błędzie? Może być ryzykowne w locie.
    }
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

DataLogger dataLogger;

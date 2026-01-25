#ifndef PATTERNS_H
#define PATTERNS_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// TYPY WZORCÓW
// ============================================================================

enum PatternType {
    PATTERN_P1A = 0,   // Przerywana długa
    PATTERN_P1B,       // Przerywana krótka
    PATTERN_P1C,       // Wydzielająca
    PATTERN_P1D,       // Prowadząca wąska
    PATTERN_P1E,       // Prowadząca szeroka
    PATTERN_P2A,       // Ciągła wąska
    PATTERN_P2B,       // Ciągła szeroka
    PATTERN_P3A,       // Przekraczalna długa
    PATTERN_P3B,       // Przekraczalna krótka
    PATTERN_P4,        // Podwójna ciągła
    PATTERN_P6,        // Ostrzegawcza
    PATTERN_P7A,       // Krawędziowa przerywana szeroka
    PATTERN_P7B,       // Krawędziowa ciągła szeroka
    PATTERN_P7C,       // Krawędziowa przerywana wąska
    PATTERN_P7D,       // Krawędziowa ciągła wąska
    PATTERN_NONE       // Brak wzorca
};

// ============================================================================
// STRUKTURA WZORCA
// ============================================================================

struct Pattern {
    PatternType type;
    const char* name;           // Nazwa wzorca (np. "P-1a")
    const char* description;    // Opis (np. "Przerywana długa")
    float lineLength_cm;        // Długość linii [cm] (0 = ciągła)
    float gapLength_cm;         // Długość przerwy [cm] (0 = brak)
    float width_cm;             // Szerokość [cm]
    uint8_t gunMask;            // Maska pistoletów (bity 0-5)
    bool canReverse;            // Czy można odwrócić (P-3a/P-3b)
    bool isDouble;              // Czy podwójna linia (P-4)
};

// ============================================================================
// KLASA ZARZĄDZANIA WZORCAMI
// ============================================================================

class PatternManager {
public:
    PatternManager();
    
    // Inicjalizacja
    void begin();
    
    // Wybór wzorca
    void selectPattern(PatternType type);
    PatternType getCurrentPattern() const { return currentPattern; }
    const Pattern* getCurrentPatternData() const;
    
    // Odwracanie wzorców P-3a/P-3b
    void setReversed(bool reversed);
    void toggleReversed();  // Przełączanie odwrócenia
    bool isReversed() const { return reversed_; }
    
    // Start Gap - rozpoczęcie od przerwy
    void setStartGap(bool enabled);
    bool isStartGapActive() const { return startGapEnabled_; }
    
    // Aktualizacja stanu malowania na podstawie przebytego dystansu
    void update(float distanceCm);
    
    // Pobieranie maski aktywnych pistoletów
    uint8_t getActiveGunMask() const { return activeGunMask; }
    
    // Resetowanie stanu wzorca
    void reset();
    
    // Obliczanie wymalowanych m²
    float calculatePaintedArea(float distanceCm) const;
    
    // Pobieranie informacji o wzorcu
    const char* getPatternName() const;
    const char* getPatternDescription() const;
    
    // Sprawdzanie czy wzorzec jest aktywny
    bool isPatternActive() const { return currentPattern != PATTERN_NONE; }
    
private:
    PatternType currentPattern;
    bool reversed_;              // Odwrócenie dla P-3a/P-3b
    bool startGapEnabled_;       // Czy Start Gap jest włączony
    float startGapDistance_cm;   // Dystans przerwy dla Start Gap
    float cyclePosition_cm;      // Pozycja w cyklu linia+przerwa [cm]
    uint8_t activeGunMask;       // Aktualna maska aktywnych pistoletów
    bool inLinePhase;            // Czy jesteśmy w fazie malowania linii
    bool startGapCompleted_;     // Czy przerwa Start Gap została ukończona
    
    // Tablica wszystkich wzorców
    static const Pattern patterns[];
    
    // Aktualizacja maski pistoletów
    void updateGunMask();
    
    // Obliczanie maski dla wzorców podwójnych
    uint8_t getDoubleMask() const;
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern PatternManager patternManager;

#endif // PATTERNS_H

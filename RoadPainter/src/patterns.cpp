#include "patterns.h"

// ============================================================================
// DEFINICJE WZORCÓW
// ============================================================================

const Pattern PatternManager::patterns[] = {
    // type, name, description, lineLength_cm, gapLength_cm, width_cm, gunMask, canReverse, isDouble
    {PATTERN_P1A, "P-1a", "Przerywana długa", 400.0, 800.0, 12.0, 0b00000111, false, false},
    {PATTERN_P1B, "P-1b", "Przerywana krótka", 200.0, 400.0, 12.0, 0b00000111, false, false},
    {PATTERN_P1C, "P-1c", "Wydzielająca", 200.0, 200.0, 12.0, 0b00000111, false, false},
    {PATTERN_P1D, "P-1d", "Prowadząca wąska", 100.0, 100.0, 12.0, 0b00000111, false, false},
    {PATTERN_P1E, "P-1e", "Prowadząca szeroka", 100.0, 100.0, 24.0, 0b00111111, false, false},
    {PATTERN_P2A, "P-2a", "Ciągła wąska", 0.0, 0.0, 12.0, 0b00000111, false, false},
    {PATTERN_P2B, "P-2b", "Ciągła szeroka", 0.0, 0.0, 24.0, 0b00111111, false, false},
    {PATTERN_P3A, "P-3a", "Przekraczalna długa", 400.0, 200.0, 12.0, 0b00000111, true, false},
    {PATTERN_P3B, "P-3b", "Przekraczalna krótka", 100.0, 100.0, 12.0, 0b00000111, true, false},
    {PATTERN_P4, "P-4", "Podwójna ciągła", 0.0, 0.0, 24.0, 0b00111111, false, true},
    {PATTERN_P6, "P-6", "Ostrzegawcza", 400.0, 200.0, 12.0, 0b00000111, false, false},
    {PATTERN_P7A, "P-7a", "Krawędziowa przeryw. szer.", 100.0, 100.0, 24.0, 0b00111111, false, false},
    {PATTERN_P7B, "P-7b", "Krawędziowa ciągła szer.", 0.0, 0.0, 24.0, 0b00111111, false, false},
    {PATTERN_P7C, "P-7c", "Krawędziowa przeryw. wąska", 100.0, 100.0, 12.0, 0b00000111, false, false},
    {PATTERN_P7D, "P-7d", "Krawędziowa ciągła wąska", 0.0, 0.0, 12.0, 0b00000111, false, false},
};

// ============================================================================
// KONSTRUKTOR
// ============================================================================

PatternManager::PatternManager() 
    : currentPattern(PATTERN_NONE)
    , reversed_(false)
    , cyclePosition_cm(0.0)
    , activeGunMask(0)
    , inLinePhase(true)
{
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

void PatternManager::begin() {
    reset();
}

// ============================================================================
// WYBÓR WZORCA
// ============================================================================

void PatternManager::selectPattern(PatternType type) {
    if (type >= PATTERN_NONE) {
        currentPattern = PATTERN_NONE;
        activeGunMask = 0;
        return;
    }
    
    currentPattern = type;
    reset();
    updateGunMask();
}

const Pattern* PatternManager::getCurrentPatternData() const {
    if (currentPattern == PATTERN_NONE) {
        return nullptr;
    }
    return &patterns[currentPattern];
}

// ============================================================================
// ODWRACANIE WZORCÓW P-3a/P-3b
// ============================================================================

void PatternManager::setReversed(bool reversed) {
    const Pattern* pattern = getCurrentPatternData();
    if (pattern && pattern->canReverse) {
        reversed_ = reversed;
        updateGunMask();
    }
}

// ============================================================================
// AKTUALIZACJA STANU MALOWANIA
// ============================================================================

void PatternManager::update(float distanceCm) {
    const Pattern* pattern = getCurrentPatternData();
    if (!pattern) {
        activeGunMask = 0;
        return;
    }
    
    // Wzorce ciągłe - zawsze aktywne
    if (pattern->lineLength_cm == 0.0) {
        updateGunMask();
        return;
    }
    
    // Wzorce przerywane - cykl linia + przerwa
    cyclePosition_cm += distanceCm;
    
    float cycleLength = pattern->lineLength_cm + pattern->gapLength_cm;
    
    // Normalizacja pozycji w cyklu
    while (cyclePosition_cm >= cycleLength) {
        cyclePosition_cm -= cycleLength;
    }
    
    // Określenie czy jesteśmy w fazie linii czy przerwy
    if (cyclePosition_cm < pattern->lineLength_cm) {
        inLinePhase = true;
    } else {
        inLinePhase = false;
    }
    
    updateGunMask();
}

// ============================================================================
// AKTUALIZACJA MASKI PISTOLETÓW
// ============================================================================

void PatternManager::updateGunMask() {
    const Pattern* pattern = getCurrentPatternData();
    if (!pattern) {
        activeGunMask = 0;
        return;
    }
    
    // Dla wzorców przerywanych - sprawdź fazę
    if (pattern->lineLength_cm > 0.0 && !inLinePhase) {
        activeGunMask = 0;
        return;
    }
    
    // Wzorzec podwójny (P-4)
    if (pattern->isDouble) {
        activeGunMask = getDoubleMask();
        return;
    }
    
    // Wzorce przekraczalne z odwracaniem (P-3a/P-3b)
    if (pattern->canReverse) {
        if (reversed_) {
            // Linia ciągła po prawej, przerywana po lewej
            if (inLinePhase || pattern->lineLength_cm == 0.0) {
                activeGunMask = 0b00111000; // Pistolety 4,5,6 (prawa strona)
            } else {
                activeGunMask = 0b00000111; // Pistolety 1,2,3 (lewa strona)
            }
        } else {
            // Linia ciągła po lewej, przerywana po prawej
            if (inLinePhase || pattern->lineLength_cm == 0.0) {
                activeGunMask = 0b00000111; // Pistolety 1,2,3 (lewa strona)
            } else {
                activeGunMask = 0b00111000; // Pistolety 4,5,6 (prawa strona)
            }
        }
        return;
    }
    
    // Standardowe wzorce
    activeGunMask = pattern->gunMask;
}

// ============================================================================
// MASKA DLA WZORCA PODWÓJNEGO
// ============================================================================

uint8_t PatternManager::getDoubleMask() const {
    // P-4: dwie ciągłe linie 24cm (2x12cm) z odstępem
    // Pistolety 1,2,3 = lewa linia
    // Pistolety 4,5,6 = prawa linia
    return 0b00111111; // Wszystkie pistolety
}

// ============================================================================
// RESETOWANIE STANU
// ============================================================================

void PatternManager::reset() {
    cyclePosition_cm = 0.0;
    inLinePhase = true;
    updateGunMask();
}

// ============================================================================
// OBLICZANIE WYMALOWANEJ POWIERZCHNI
// ============================================================================

float PatternManager::calculatePaintedArea(float distanceCm) const {
    const Pattern* pattern = getCurrentPatternData();
    if (!pattern) {
        return 0.0;
    }
    
    // Powierzchnia = szerokość × dystans
    // Dla wzorców przerywanych uwzględnij współczynnik pokrycia
    float coverage = 1.0;
    
    if (pattern->lineLength_cm > 0.0) {
        float cycleLength = pattern->lineLength_cm + pattern->gapLength_cm;
        coverage = pattern->lineLength_cm / cycleLength;
    }
    
    // Dla wzorca podwójnego (P-4) - dwie linie
    float widthMultiplier = pattern->isDouble ? 2.0 : 1.0;
    
    // Powierzchnia w cm²
    float areaCm2 = distanceCm * pattern->width_cm * coverage * widthMultiplier;
    
    // Konwersja na m²
    return areaCm2 * CM_TO_M2_FACTOR;
}

// ============================================================================
// POBIERANIE INFORMACJI O WZORCU
// ============================================================================

const char* PatternManager::getPatternName() const {
    const Pattern* pattern = getCurrentPatternData();
    return pattern ? pattern->name : "---";
}

const char* PatternManager::getPatternDescription() const {
    const Pattern* pattern = getCurrentPatternData();
    return pattern ? pattern->description : "Brak wzorca";
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

PatternManager patternManager;

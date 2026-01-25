#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// KONFIGURACJA PINÓW ESP32-S3
// ============================================================================

// Wyświetlacz ILI9341 (SPI)
#define TFT_MOSI      11
#define TFT_MISO      13
#define TFT_SCLK      12
#define TFT_CS        10
#define TFT_DC        9
#define TFT_RST       8
#define TFT_BL        14  // Backlight

// Enkoder KY-040
#define ENCODER_CLK   4
#define ENCODER_DT    5
#define ENCODER_SW    6

// Joystick
#define JOYSTICK_VRX  1   // ADC1_CH0
#define JOYSTICK_VRY  2   // ADC1_CH1
#define JOYSTICK_SW   7

// Przyciski sterowania
#define BTN_START_PAUSE  15
#define BTN_STOP         16

// Przyciski wyboru wzorców (13 wzorców)
#define BTN_P1A      17
#define BTN_P1B      18
#define BTN_P1C      21
#define BTN_P1D      47
#define BTN_P1E      48
#define BTN_P2A      45
#define BTN_P2B      46
#define BTN_P3A      38
#define BTN_P3B      39
#define BTN_P4       40
#define BTN_P6       41
#define BTN_P7A      42
#define BTN_P7B      2
#define BTN_P7C      3
#define BTN_P7D      37

// Przekaźniki (6 pistoletów malarskich)
#define RELAY_1      35
#define RELAY_2      36
#define RELAY_3      33
#define RELAY_4      34
#define RELAY_5      26
#define RELAY_6      25

// ============================================================================
// PARAMETRY ENKODERA
// ============================================================================

#define ENCODER_PULSES_PER_REV  20      // Impulsy na obrót (KY-040)
#define WHEEL_DIAMETER_MM       100.0   // Średnica koła pomiarowego [mm]
#define WHEEL_CIRCUMFERENCE_MM  (WHEEL_DIAMETER_MM * PI)

// ============================================================================
// PARAMETRY WYŚWIETLACZA
// ============================================================================

#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240

// Kolory (RGB565)
#define COLOR_BG           0x0000  // Czarny
#define COLOR_TEXT         0xFFFF  // Biały
#define COLOR_HEADER       0x07FF  // Cyan
#define COLOR_ACTIVE       0x07E0  // Zielony
#define COLOR_INACTIVE     0xF800  // Czerwony
#define COLOR_WARNING      0xFFE0  // Żółty
#define COLOR_MENU_BG      0x18E3  // Ciemny szary
#define COLOR_MENU_SELECT  0x051D  // Niebieski

// ============================================================================
// PARAMETRY CZASOWE
// ============================================================================

#define DISPLAY_REFRESH_MS     100    // Odświeżanie wyświetlacza [ms]
#define SPEED_CALC_INTERVAL_MS 200    // Obliczanie prędkości [ms]
#define DEBOUNCE_MS            50     // Debouncing przycisków [ms]
#define LONG_PRESS_MS          1000   // Długie naciśnięcie [ms]
#define VERY_LONG_PRESS_MS     2000   // Bardzo długie naciśnięcie [ms]

// ============================================================================
// PARAMETRY MALOWANIA
// ============================================================================

// Szerokość pojedynczego pistoletu [cm]
#define SINGLE_GUN_WIDTH_CM    4.0

// Konfiguracja pistoletów dla różnych szerokości
// 12cm = 3 pistolety (1, 2, 3)
// 24cm = 6 pistoletów (wszystkie)

// ============================================================================
// PAMIĘĆ EEPROM
// ============================================================================

#define EEPROM_SIZE            512
#define EEPROM_ADDR_CALIBRATION 0     // Współczynnik kalibracji (float, 4 bajty)
#define EEPROM_ADDR_INITIALIZED 4     // Flaga inicjalizacji (byte)
#define EEPROM_MAGIC_BYTE      0xAA   // Znacznik poprawnej inicjalizacji

// ============================================================================
// STAŁE SYSTEMOWE
// ============================================================================

#define CALIBRATION_DISTANCE_CM  1000.0  // 10 metrów dla kalibracji
#define CM_TO_M2_FACTOR          0.0001  // Przelicznik cm² na m²

// Maksymalna prędkość [km/h] (zabezpieczenie)
#define MAX_SPEED_KMH           30.0

#endif // CONFIG_H

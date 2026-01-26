#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include "patterns.h"
#include "encoder.h"
#include "ui_manager.h"

// ============================================================================
// KONFIGURACJA WIFI
// ============================================================================

#define WIFI_SSID "Trassar"
#define WIFI_PASSWORD "12345678"
#define WIFI_CHANNEL 1
#define MAX_CLIENTS 4

// ============================================================================
// KLASA SERWERA WWW
// ============================================================================

class WebServerManager {
public:
    WebServerManager();
    
    // Inicjalizacja
    void begin();
    
    // Aktualizacja (wywołuj w loop)
    void update();
    
    // Pobieranie adresu IP
    String getIPAddress() const;
    
    // Sprawdzanie czy klient jest podłączony
    bool isClientConnected() const;
    
private:
    WebServer server;
    unsigned long lastUpdate_ms;
    
    // Obsługa żądań HTTP
    void handleRoot();
    void handleStatus();
    void handleControl();
    void handlePatternSelect();
    void handleStartPause();
    void handleStop();
    void handleStartGap();
    void handleStartGap();
    void handleReverseP3();
    void handleNotFound();
    
    // OTA Update
    void handleUpdate();
    void handleUpdateUpload();
    
    // Generowanie HTML
    String generateHTML();
    String generateStatusJSON();
};

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

extern WebServerManager webServer;

#endif // WEB_SERVER_H

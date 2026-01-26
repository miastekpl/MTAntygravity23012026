#include "web_server.h"

// ============================================================================
// HTML DASHBOARD
// ============================================================================

const char* INDEX_HTML = R"rawliteral(
<!DOCTYPE html>
<html lang="pl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RoadPainter Remote</title>
    <style>
        body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #121212; color: #e0e0e0; margin: 0; padding: 20px; }
        .container { max-width: 600px; margin: 0 auto; }
        .card { background-color: #1e1e1e; border-radius: 10px; padding: 20px; margin-bottom: 20px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }
        .header { text-align: center; color: #00bcd4; margin-bottom: 20px; }
        .value-box { display: flex; justify-content: space-between; margin-bottom: 10px; }
        .value { font-size: 1.2em; font-weight: bold; }
        .btn { width: 100%; padding: 15px; border: none; border-radius: 8px; font-size: 1.1em; font-weight: bold; cursor: pointer; margin-bottom: 10px; transition: 0.2s; }
        .btn-start { background-color: #4caf50; color: white; }
        .btn-stop { background-color: #f44336; color: white; }
        .btn:active { transform: scale(0.98); }
        .pattern-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; }
        .btn-pat { background-color: #333; color: #aaa; padding: 10px; font-size: 0.9em; }
        .btn-pat.active { background-color: #00bcd4; color: white; }
        .status-badge { display: inline-block; padding: 5px 10px; border-radius: 5px; background: #333; }
        .status-idle { background: #555; }
        .status-work { background: #4caf50; animation: pulse 1s infinite; }
        @keyframes pulse { 0% { opacity: 1; } 50% { opacity: 0.7; } 100% { opacity: 1; } }
    </style>
</head>
<body>
    <div class="container">
        <h2 class="header">SYSTEM MALOWANIA TRASSAR</h2>
        
        <div class="card">
            <div class="value-box">
                <span>Status:</span>
                <span id="status" class="status-badge status-idle">OCZEKIWANIE</span>
            </div>
            <div class="value-box">
                <span>Prędkość:</span>
                <span class="value"><span id="speed">0.0</span> km/h</span>
            </div>
            <div class="value-box">
                <span>Dystans:</span>
                <span class="value"><span id="dist">0.0</span> m</span>
            </div>
            <div class="value-box">
                <span>Powierzchnia:</span>
                <span class="value"><span id="area">0.00</span> m²</span>
            </div>
        </div>

        <div class="card">
            <button class="btn btn-start" onclick="sendCommand('start')">START / PAUZA</button>
            <button class="btn btn-stop" onclick="sendCommand('stop')">STOP</button>
            
            <div style="margin-top: 15px; display: flex; gap: 10px;">
                <button class="btn" style="background: #ff9800; color: white;" onclick="sendCommand('start_gap')">START GAP</button>
                <button class="btn" style="background: #9c27b0; color: white;" onclick="sendCommand('reverse_p3')">ODWRÓĆ P-3</button>
            </div>
        </div>

        <div class="card">
            <h3>Wybór Wzorca</h3>
            <div class="pattern-grid" id="patterns">
                <!-- Wzorce generowane przez JS -->
            </div>
        </div>
    </div>

    <script>
        const patterns = [
            "P-1a", "P-1b", "P-1c", "P-1d", "P-1e",
            "P-2a", "P-2b",
            "P-3a", "P-3b",
            "P-4", "P-6",
            "P-7a", "P-7b", "P-7c", "P-7d"
        ];

        function initPatterns() {
            const grid = document.getElementById('patterns');
            patterns.forEach((p, i) => {
                grid.innerHTML += `<button id="pat-${i}" class="btn btn-pat" onclick="selectPattern(${i})">${p}</button>`;
            });
        }

        function updateData() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('speed').innerText = data.speed.toFixed(1);
                    document.getElementById('dist').innerText = (data.dist / 100.0).toFixed(1);
                    document.getElementById('area').innerText = data.area.toFixed(2);
                    
                    const statusEl = document.getElementById('status');
                    if(data.state === 1) { // PAINTING
                        statusEl.innerText = "MALOWANIE";
                        statusEl.className = "status-badge status-work";
                    } else if(data.state === 2) { // PAUSED
                        statusEl.innerText = "PAUZA";
                        statusEl.className = "status-badge status-idle";
                    } else {
                        statusEl.innerText = "GOTOWY";
                        statusEl.className = "status-badge status-idle";
                    }

                    // Update active pattern
                    document.querySelectorAll('.btn-pat').forEach(btn => btn.classList.remove('active'));
                    if(data.pattern >= 0) {
                        const btn = document.getElementById('pat-' + data.pattern);
                        if(btn) btn.classList.add('active');
                    }
                });
        }

        function sendCommand(cmd, val) {
            let url = '/control?cmd=' + cmd;
            if(val !== undefined) url += '&val=' + val;
            fetch(url);
        }

        function selectPattern(id) {
            sendCommand('pattern', id);
        }

        initPatterns();
        setInterval(updateData, 500);
    </script>
</body>
</html>
)rawliteral";

// ============================================================================
// KONSTRUKTOR
// ============================================================================

WebServerManager::WebServerManager() 
    : server(80)
    , lastUpdate_ms(0)
{
}

// ============================================================================
// INICJALIZACJA
// ============================================================================

void WebServerManager::begin() {
    // Konfiguracja Access Point
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL, 0, MAX_CLIENTS);
    
    Serial.println("WebServer: Access Point uruchomiony");
    Serial.printf("SSID: %s\n", WIFI_SSID);
    Serial.printf("IP: %s\n", WiFi.softAPIP().toString().c_str());
    
    // Konfiguracja ścieżek
    server.on("/", HTTP_GET, [this]() { handleRoot(); });
    server.on("/status", HTTP_GET, [this]() { handleStatus(); });
    server.on("/control", HTTP_GET, [this]() { handleControl(); });
    
    // OTA Update
    server.on("/update", HTTP_GET, [this]() { handleUpdate(); });
    server.on("/update", HTTP_POST, [this]() {
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, [this]() { handleUpdateUpload(); });
    
    server.onNotFound([this]() { handleNotFound(); });
    
    server.begin();
    Serial.println("WebServer: Serwer HTTP wystartował");
}

// ============================================================================
// AKTUALIZACJA
// ============================================================================

void WebServerManager::update() {
    server.handleClient();
}

// ============================================================================
// OBSŁUGA ŻĄDAŃ
// ============================================================================

void WebServerManager::handleRoot() {
    server.send(200, "text/html", INDEX_HTML);
}

void WebServerManager::handleStatus() {
    server.send(200, "application/json", generateStatusJSON());
}

// OTA Update Handlers
void WebServerManager::handleUpdate() {
    String html = "<form method='POST' action='/update' enctype='multipart/form-data'>";
    html += "<input type='file' name='update'>";
    html += "<input type='submit' value='Update'>";
    html += "</form>";
    server.send(200, "text/html", html);
}

void WebServerManager::handleUpdateUpload() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // start with max available size
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
    }
}

void WebServerManager::handleControl() {
    if (!server.hasArg("cmd")) {
        server.send(400, "text/plain", "Missing command");
        return;
    }
    
    String cmd = server.arg("cmd");
    
    if (cmd == "start") {
        uiManager.handleStartPause();
    } else if (cmd == "stop") {
        uiManager.handleStop(PRESS_SHORT);
    } else if (cmd == "pattern") {
        if (server.hasArg("val")) {
            int patId = server.arg("val").toInt();
            uiManager.handlePatternButton((PatternType)patId);
        }
    } else if (cmd == "start_gap") {
        patternManager.setStartGap(!patternManager.isStartGapActive());
    } else if (cmd == "reverse_p3") {
        patternManager.toggleReversed();
    }
    
    server.send(200, "text/plain", "OK");
}

void WebServerManager::handleNotFound() {
    server.send(404, "text/plain", "Not found");
}

// ============================================================================
// GENEROWANIE JSON
// ============================================================================

String WebServerManager::generateStatusJSON() {
    String json = "{";
    json += "\"speed\":" + String(encoderManager.getSpeed_kmh()) + ",";
    json += "\"dist\":" + String(encoderManager.getDistanceCm()) + ",";
    json += "\"area\":" + String(uiManager.getTotalArea()) + ",";
    json += "\"state\":" + String((int)uiManager.getState()) + ",";
    json += "\"pattern\":" + String((int)patternManager.getCurrentPattern());
    json += "}";
    return json;
}

// ============================================================================
// POMOCNICZE
// ============================================================================

String WebServerManager::getIPAddress() const {
    return WiFi.softAPIP().toString();
}

bool WebServerManager::isClientConnected() const {
    return WiFi.softAPgetStationNum() > 0;
}

// ============================================================================
// GLOBALNA INSTANCJA
// ============================================================================

WebServerManager webServer;

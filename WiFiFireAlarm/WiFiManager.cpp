#include "WiFiManager.h"
#include <ArduinoJson.h>
#include "Global.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : _ssid(ssid), _password(password), _attempts(0), _connectionFailed(false) {}

void WiFiManager::begin() {
    // Only try to connect so many times before giving up
    if (_connectionFailed) {
        Serial.println("Max connection attempts reached. No further attempts.");
        return;
    }
    
    // Flash LED before attempting connection
    blinkLEDBeforeConnection();
    Serial.println("Connecting to WiFi...");
    WiFi.begin(_ssid, _password);

    // Blink LED while attempting to connect
    blinkLED(true);

    // Retry connection a limited number of times
    while (WiFi.status() != WL_CONNECTED && _attempts < 10) { 
        delay(500);
        _attempts++;
    }
    
    blinkLED(false);

    if (WiFi.status() == WL_CONNECTED) logger.print("Connected to WiFi!");
    else {
        // Set to failed after so many attempts
        logger.print("Failed to connect to WiFi.");
        _connectionFailed = true;
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::blinkLED(bool isBlinking) {
    if (isBlinking) {
        while (!isConnected()) {
            digitalWrite(LED_BUILTIN, HIGH); 
            delay(250);
            digitalWrite(LED_BUILTIN, LOW);
            delay(250);
        }
    } else {
        digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED ? HIGH : LOW);
    }
}

void WiFiManager::blinkLEDBeforeConnection() {
    unsigned long startTime = millis();
    while (millis() - startTime < 2000) {  
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
    }
}

WiFiClient& WiFiManager::getClient() {
    return _client;
}

bool isHexadecimal(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }

// The ThingSpeak library was having issues reading fields so I had to interface with the API manually
ThingSpeakData WiFiManager::retrieveDataFromThingSpeak() {
    ThingSpeakData data;

    // If not connected return null data
    if (!isConnected()) return data;

    // Connect to ThingSpeak api
    if (!_client.connect("api.thingspeak.com", 80)) return data;

    // Request public feed data
    _client.print("GET /channels/2762893/feeds.json?results=1 HTTP/1.1\r\n"
                  "Host: api.thingspeak.com\r\n"
                  "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (_client.available() == 0) {
        if (millis() - timeout > 5000) {
            _client.stop();
            return data;
        }
    }

    // Parse the data with the arduino json library
    String jsonResponse;
    bool isJsonPart = false;
    while (_client.available()) {
        String line = _client.readStringUntil('\n');
        if (line == "\r") {
            isJsonPart = true;
        } else if (isJsonPart && !(line.length() > 0 && isHexadecimal(line[0]))) {
            jsonResponse += line;
        }
    }

    // Close the client as it is no longer needed
    _client.stop();

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(10) + JSON_ARRAY_SIZE(1) + 500;
    DynamicJsonDocument doc(capacity);

    // Convert json into an object
    if (deserializeJson(doc, jsonResponse)) return data;

    // If the feed is empty return current state of data;
    if (doc["feeds"].size() == 0) return data;

    // Extract the individual fields and return the value or null
    JsonObject feed = doc["feeds"][0];
    data.field1 = feed["field1"] | "null";
    data.field2 = feed["field2"] | "null";
    data.field3 = feed["field3"] | "null";
    data.field4 = feed["field4"] | "null";
    data.field5 = feed["field5"] | "null";

    return data;
}

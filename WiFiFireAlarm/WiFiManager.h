#pragma once
#include "Arduino.h"
#include "WiFiFireAlarm.h"
#include <WiFiS3.h>

struct ThingSpeakData {
    String field1; // Flame
    String field2; // Gas
    String field3; // Muted
    String field4; // Active
    String field5; // Network Status
};

class WiFiManager {
private:
    const char* _ssid;
    const char* _password;
    unsigned int _attempts;
    bool _connectionFailed;
    WiFiClient _client;

    void blinkLED(bool isBlinking);
    void blinkLEDBeforeConnection(); 

public:
    WiFiManager(const char* ssid, const char* password);
    void begin();
    bool isConnected();
    WiFiClient& getClient();
    ThingSpeakData retrieveDataFromThingSpeak();
};
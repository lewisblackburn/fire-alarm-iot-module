#pragma once
#include "Arduino.h"
#include "ThingSpeak.h"
#include "WiFiFireAlarm.h"
#include "WiFiManager.h"
#include <map>

class ThingSpeakManager {
private:
    unsigned long _channelNumber;
    const char* _writeAPIKey;
    const char* _readAPIKey;
    WiFiManager& _wifiManager;

public:
    ThingSpeakManager(unsigned long channelNumber, const char* writeAPIKey, const char* readAPIKey, WiFiManager& wifiManager);
    void begin();
    void sendFields(const std::map<int, float>& fields);
    float readField(int fieldNumber);
};

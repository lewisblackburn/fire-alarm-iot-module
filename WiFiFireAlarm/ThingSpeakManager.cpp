#include "ThingSpeakManager.h"
#include "Global.h"

ThingSpeakManager::ThingSpeakManager(unsigned long channelNumber, const char* writeAPIKey, const char* readAPIKey, WiFiManager& wifiManager)
    : _channelNumber(channelNumber), 
      _writeAPIKey(writeAPIKey),
      _readAPIKey(readAPIKey),
      _wifiManager(wifiManager) {}

void ThingSpeakManager::begin() {
    if (_wifiManager.isConnected()) {
        Serial.println("Initialising ThingSpeak...");
        ThingSpeak.begin(_wifiManager.getClient());
        Serial.println("ThingSpeak initialised.");

        // Set network status to 1 when initialised
        thingSpeakManager.sendFields({{5, 1.0f}});
    } else {
        Serial.println("WiFi is not connected. Cannot initialise ThingSpeak.");
    }
}

void ThingSpeakManager::sendFields(const std::map<int, float>& fields) {
    for (const auto& field : fields) {
        ThingSpeak.setField(field.first, field.second);
    }
   ThingSpeak.writeFields(_channelNumber, _writeAPIKey);
}

float ThingSpeakManager::readField(int fieldNumber) {
    float value = ThingSpeak.readFloatField(_channelNumber, fieldNumber);
    return value;
}

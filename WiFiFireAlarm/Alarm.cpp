#include "Alarm.h"
#include "Global.h"
#include "WiFiFireAlarm.h"

Alarm::Alarm(int flamePin, int gasPin, int buzzerPin, int redLedPin)
    : flamePin(flamePin), gasPin(gasPin), buzzerPin(buzzerPin),
      redLedPin(redLedPin), isActive(false), flameDetected(false), gasDetected(false) {
    pinMode(flamePin, INPUT);
    pinMode(gasPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    pinMode(Pins::BUTTON, INPUT_PULLUP);
}

unsigned long lastThingSpeakFetchTime = 0;
const unsigned long fetchInterval = 15000;

// Default muted and override values
bool fireAlarmOverride = false;
bool isMuted = false;

void Alarm::checkAndActivate() {
  // Read ThingSpeak data every 15 seconds
    unsigned long currentMillis = millis();
    if (currentMillis - lastThingSpeakFetchTime >= fetchInterval) {
        lastThingSpeakFetchTime = currentMillis;
        ThingSpeakData data = wifiManager.retrieveDataFromThingSpeak();

        // Set fields based on data
        isMuted = data.field3.toFloat() == 1.0f;
        fireAlarmOverride = data.field4.toFloat() == 1.0f;
    }

    // Deactivate alarm when button pressed
    if (digitalRead(Pins::BUTTON) == LOW) deactivate();

    if (!isActive) {
        // Set flame and/or gas detected to true if sensor active
        flameDetected = (digitalRead(flamePin) == LOW);
        gasDetected = (digitalRead(gasPin) == LOW);

        // Activate/deactivate from dashboard
        if (fireAlarmOverride) activate();
        else deactivate();
        
        // Activate/deactivate based on sensor values
        if (flameDetected || gasDetected) activate();
    }

    if (isActive) soundAlarm();
}

void Alarm::activate() {  
    // Run when first activated
    if (!isActive) {
        logger.print("Alarm activated!");
        isActive = true;
        digitalWrite(redLedPin, HIGH); 

        // Field 1: Flame
        // Field 2: Gas
        // Field 3: Muted
        // Field 4: Active
        // Field 5: Network Status
        thingSpeakManager.sendFields({
          {1, flameDetected ? 1.0f : 0.0f}, 
          {2, gasDetected ? 1.0f : 0.0f},    
          {4, 1.0f},                          
          {5, 1.0f}
        }); 
    }
}

void Alarm::deactivate() {
    if (isActive) {
        logger.print("Alarm deactivated.");
        isActive = false;
        flameDetected = false;
        gasDetected = false;

        // Turn off red LED and Buzzer
        digitalWrite(redLedPin, LOW);
        digitalWrite(buzzerPin, LOW);

        // There needs to be fifteen seconds between POST requests for ThingSpeak so a delay 15000ms is set to be safe
        delay(15000);
        thingSpeakManager.sendFields({
            {1, 0.0f},
            {2, 0.0f},
            {4, 0.0f},
            {5, 1.0f}
        });
  }
}

void Alarm::soundAlarm() {
    if (isActive) {
        // If muted activate red flashing lights else activate buzzing alarm
        if (isMuted) {
          digitalWrite(redLedPin, HIGH);
          delay(500); 
          digitalWrite(redLedPin, LOW);
          delay(100); 
        } else {
          // Make sure red LED is on
          digitalWrite(redLedPin, HIGH); 
          digitalWrite(buzzerPin, HIGH);
          delay(1000); 
          digitalWrite(buzzerPin, LOW);
          delay(1000);  
        }
    }
}
#include "WiFiFireAlarm.h"
#include "Alarm.h"
#include "Global.h"
#include "WiFiS3.h"

Alarm alarm(Pins::FLAME_SENSOR, Pins::GAS_SENSOR, Pins::BUZZER, Pins::RED_LED);

void setup()
{
  // Start serialisation
  Serial.begin(Config::SERIAL_BAUD_RATE);

  // Pins
  pinMode(Pins::GREEN_LED, OUTPUT);

  // Initialisation
  display.begin();
  display.showStatus(INITIALISING);
  wifiManager.begin();

  // Give the WiFI Manager a chance to initiate properly
  delay(10000);
  thingSpeakManager.begin();
  sdManager.begin();
  display.showStatus(INITIALISED);
  display.clear();
  logger.print("Ready.");
}

void loop()
{
  if (wifiManager.isConnected()) digitalWrite(Pins::GREEN_LED, HIGH);
  alarm.checkAndActivate();
  delay(Config::MAIN_LOOP_DELAY);
}
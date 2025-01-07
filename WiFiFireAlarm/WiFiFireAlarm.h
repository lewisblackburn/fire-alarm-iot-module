#ifndef WIFI_FIRE_ALARM_H
#define WIFI_FIRE_ALARM_H

#include <Arduino.h>

namespace Pins
{
    const int FLAME_SENSOR = 1;
    const int GAS_SENSOR = 2;
    const int BUZZER = 8;
    const int RED_LED = 5;
    const int GREEN_LED = 4;
    const int BUTTON = 7;
}

namespace Config
{
    static const char *WIFI_SSID = "";
    static const char *WIFI_PASSWORD = "";
    static unsigned long CHANNEL_NUMBER = 2762893;
    static const char *READ_API_KEY = "";
    static const char *WRITE_API_KEY = "";
    const int SERIAL_BAUD_RATE = 9600;
    const int WIFI_MAX_ATTEMPTS = 10;
    const int INITIAL_BLINK_DURATION = 2000;
    const int ALARM_SOUND_DURATION = 500;
    const int MAIN_LOOP_DELAY = 1000;
}

#endif
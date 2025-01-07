#include "Global.h"

#define CHIP_SELECT_PIN 10 

SDCardManager sdManager(CHIP_SELECT_PIN);
DisplayManager display;
WiFiManager wifiManager(Config::WIFI_SSID, Config::WIFI_PASSWORD);
ThingSpeakManager thingSpeakManager(Config::CHANNEL_NUMBER, Config::WRITE_API_KEY, Config::READ_API_KEY, wifiManager);
Logger logger;
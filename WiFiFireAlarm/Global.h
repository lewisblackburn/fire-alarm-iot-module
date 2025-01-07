#ifndef GLOBAL_H
#define GLOBAL_H

#include "DisplayManager.h"
#include "SDCardManager.h"
#include "WiFiManager.h"
#include "ThingSpeakManager.h"
#include "Logger.h"

extern DisplayManager display;
extern SDCardManager sdManager;
extern WiFiManager wifiManager;
extern ThingSpeakManager thingSpeakManager;
extern Logger logger;

#endif

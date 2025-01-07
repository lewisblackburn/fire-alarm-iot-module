#include <Arduino.h>
#include "Logger.h"
#include "Global.h"

Logger::Logger() {}

void Logger::print(const char* logMessage) {
    Serial.println(logMessage);
    display.showMessage(logMessage, 0);
    sdManager.writeLog("LOG.txt", logMessage);
}
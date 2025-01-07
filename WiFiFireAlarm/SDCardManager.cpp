#include "SDCardManager.h"
#include "Global.h"

SDCardManager::SDCardManager(uint8_t chipSelectPin) : _chipSelectPin(chipSelectPin) {}

void SDCardManager::begin() {
   if (SD.begin(_chipSelectPin)) logger.print("SD card initialised.");
   else logger.print("SD card initialisation failed.");
}

bool SDCardManager::writeLog(const char* filename, const String& logMessage) {
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        return false;
    }

    // Time in milliseconds since startup
    unsigned long currentTime = millis(); 
    String logWithTime = String("[") + currentTime + "] " + logMessage;

    file.println(logWithTime);
    file.close();
    return true;
}

String SDCardManager::readLog(const char* filename) {
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return "";
    }

    String content = "";
    while (file.available()) {
        content += char(file.read());
    }
    file.close();
    return content;
}

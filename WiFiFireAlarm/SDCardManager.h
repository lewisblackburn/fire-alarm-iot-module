#ifndef SDCARDMANAGER_H
#define SDCARDMANAGER_H

#include <SD.h>
#include <SPI.h>

class SDCardManager {
private:
    uint8_t _chipSelectPin;
    
public:
    SDCardManager(uint8_t chipSelectPin);

    void begin();
    bool writeLog(const char* filename, const String& logMessage);
    String readLog(const char* filename);
};

#endif 

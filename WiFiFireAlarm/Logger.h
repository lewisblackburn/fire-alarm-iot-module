#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger {
public:
    Logger(); 
    void print(const char* logMessage);
};

#endif

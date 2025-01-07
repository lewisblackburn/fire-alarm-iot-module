#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <LiquidCrystal_I2C.h>

enum DisplayStatus {
    INITIALISING,
    INITIALISED,
};

class DisplayManager {
private:
    LiquidCrystal_I2C _lcd;

    const char* getStatusMessage(DisplayStatus status);

public:
    DisplayManager();
    void begin();
    void clear();
    void showMessage(const char* message, int row = 0);
    void showValue(float value, int row = 1);
    void showError(const char* error, int errorCode);
    void showStatus(DisplayStatus status, float value = 0);
};

#endif

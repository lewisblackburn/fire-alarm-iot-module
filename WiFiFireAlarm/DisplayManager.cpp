#include "DisplayManager.h"
#include <Wire.h>

DisplayManager::DisplayManager() : _lcd(0x27, 16, 2) {}

void DisplayManager::begin() {
    Wire.begin();
    _lcd.init();
    _lcd.backlight();
    _lcd.clear();
}

void DisplayManager::clear() {
    delay(1000);
    _lcd.clear();
}

void DisplayManager::showMessage(const char* message, int row) {
    _lcd.clear();
    _lcd.setCursor(0, row);
    _lcd.print(message);
}

void DisplayManager::showValue(float value, int row) {
    _lcd.setCursor(0, row);
    _lcd.print(value);
}

void DisplayManager::showError(const char* error, int errorCode) {
    clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Error: ");
    _lcd.print(error);
    _lcd.setCursor(0, 1);
    _lcd.print("Code: ");
    _lcd.print(errorCode);
}

void DisplayManager::showStatus(DisplayStatus status, float value) {
    clear();
    showMessage(getStatusMessage(status), 0);
    if (value != 0) {
        showValue(value, 1);
    }
}

const char* DisplayManager::getStatusMessage(DisplayStatus status) {
    switch (status) {
        case INITIALISING: return "Initialising...";
        case INITIALISED: return "Initialised";
        default: return "Unknown Status";
    }
}

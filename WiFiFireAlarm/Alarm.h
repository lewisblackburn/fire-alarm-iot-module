#ifndef ALARM_H
#define ALARM_H

class Alarm
{
private:
    const int flamePin;  
    const int gasPin;    
    const int buzzerPin;  
    const int redLedPin;   
    bool isActive;         
    bool flameDetected;    
    bool gasDetected;     

    void soundAlarm();     

public:
    Alarm(int flamePin, int gasPin, int buzzerPin, int redLedPin);

    void checkAndActivate();
    void activate();   
    void deactivate();       

    // Helper getter functions
    bool isFlameDetected() const { return flameDetected; } 
    bool isGasDetected() const { return gasDetected; }   
    bool getAlarmStatus() const { return isActive; }
};

#endif

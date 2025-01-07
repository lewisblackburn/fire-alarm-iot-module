#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(32, 16, 2);

int BUTTON = 2;
int BUZZER = 13;
int RED_LED = 12;
int GREEN_LED = 11;
int GAS_SENSOR = A0;
bool active = false;
bool alarmDeactivated = false;
unsigned long deactivationTime = 0;  
unsigned long resetDelay = 5000;  

int counter;

void setup()
{
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(GAS_SENSOR, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);

  for (counter = 0; counter < 5; ++counter) {
    digitalWrite(GREEN_LED, HIGH);
    delay(250); // Wait for 250 millisecond(s)
    digitalWrite(GREEN_LED, LOW);
    delay(250);
  }
  digitalWrite(GREEN_LED, HIGH);
}

void loop()
{
  lcd.clear();  
  lcd.setCursor(0, 0);
  
  float gasSensor = analogRead(GAS_SENSOR);
  
  lcd.print("Gas Detected: ");
  if (active) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    lcd.print("Y");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    lcd.print("N");
  }
  
  if (!alarmDeactivated && gasSensor >= 350) {
    active = true;
  }

  int buttonState = digitalRead(BUTTON);
  Serial.println(buttonState);

  if (buttonState == HIGH) {
    active = false;
    alarmDeactivated = true;  
    deactivationTime = millis();  
  }

  if (alarmDeactivated && millis() - deactivationTime >= resetDelay) {
    alarmDeactivated = false;
  }



  delay(1000); // Update the display every 1 second
}

// Attiny85 (2-PB3, 3-PB4, 5-PB0, 6-PB1, 7-PB2)
#include <Arduino.h>

#define but_tap 3
#define sens 4

void onBat();
boolean stat_fly = false;

void setup()
{
  pinMode(but_tap, OUTPUT);
  digitalWrite(but_tap, LOW);
  pinMode(sens, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(sens) && !stat_fly) // Если напряжение низкое
  {

    delay(1000);
    if (digitalRead(sens) && !stat_fly)
    {
      stat_fly = true;
      onBat();
      delay(5000);
      return;
    }

    return;
  }

  // Выключить после полета
  if (!digitalRead(sens) && stat_fly) // Если напряжение высокое
  {
    delay(10000);
    if (!digitalRead(sens) && stat_fly)
    {
      onBat();
    }
    return;
  }
}

void onBat()
{
  digitalWrite(but_tap, HIGH);
  delay(200);
  digitalWrite(but_tap, LOW);
  delay(300);
  digitalWrite(but_tap, HIGH);
  delay(2000);
  digitalWrite(but_tap, LOW);
  delay(2000);
}

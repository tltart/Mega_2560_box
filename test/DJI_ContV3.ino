// Attiny85 (2-PB3, 3-PB4, 5-PB0, 6-PB1, 7-PB2)
#include <Arduino.h>

#define but_tap 3
#define sens 1

void onBat();
unsigned long previousMillis;
const unsigned long interval = 20000;
boolean stat = false;
boolean base = false;


void setup()
{
  pinMode(but_tap, OUTPUT);
  digitalWrite(but_tap, HIGH);
  pinMode(sens, INPUT);
}

void loop()
{
  previousMillis = millis();

  if (!digitalRead(sens) && !stat)        // Появилось питание и статус не в базе
    while (!base)                         // Пока статус не в базе
    {
      if (millis() - previousMillis > interval && !digitalRead(sens))   // Если прошло больше интервала, то заходим и делаем статус в БАЗЕ
      {
        base = true;
        return;
      }
      if (millis() - previousMillis < interval && digitalRead(sens))  // Если прошло меньше интервала и пропало питание, то включаем.
      {
        delay(3000);
        if (digitalRead(sens) && !stat && !base)
        {
          onBat();
          stat = true;
          return;
        }
      }
    }

  // Выключить после полета
  if (!digitalRead(sens) && !base && stat)
  {
    delay(2000);
    if (!digitalRead(sens))
    {
      onBat();
      stat = false;
      return;
    }
    return;
  }
}

void onBat()
{
  digitalWrite(but_tap, LOW);
  delay(300);
  digitalWrite(but_tap, HIGH);
  delay(600);
  digitalWrite(but_tap, LOW);
  delay(3000);
  digitalWrite(but_tap, HIGH);
  delay(2000);
}

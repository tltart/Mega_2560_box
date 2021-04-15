// Attiny85 (2-PB3, 3-PB4, 5-PB0, 6-PB1, 7-PB2)
#include <Arduino.h>

#define but_tap 3
#define sens 1

void onBat();
unsigned long previousMillis;
const unsigned long interval = 20000;
boolean stat_fly = false;
boolean on_but = true;



void setup()
{
  pinMode(but_tap, OUTPUT);
  digitalWrite(but_tap, HIGH);
  pinMode(sens, INPUT);
  onBat();
  previousMillis = millis();
}

void loop()
{
  if (millis() - previousMillis > interval && !digitalRead(sens) && on_but && !stat_fly)   // Если прошло больше интервала и есть питание, то выключаем батарею
  {
    onBat();
    on_but = false;
    return;
  }
  if (millis() - previousMillis < interval && digitalRead(sens) && !stat_fly)  // Если прошло меньше интервала и пропало питание и статус не полет.
  {
    delay(2000);                       //задержка для проверки отсутсвия питания
    if (digitalRead(sens))
    {
      stat_fly = true;                 //меняем статус на полет
      return;
    }
  }
  // Выключить после полета
  if (!digitalRead(sens) && stat_fly)  // Если появилось питание и статус в полете
  {
    delay(2000);
    if (!digitalRead(sens))
    {
      onBat();
      return;
    }
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

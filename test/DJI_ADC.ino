// Attiny85 (2-PB3, 3-PB4, 5-PB0, 6-PB1, 7-PB2)
#include <Arduino.h>


#define led 3
#define sens 2
#define MEASURE_PERIOD 500

int val;
int por = 300;
int timeCount = 0;
signed long  sum;
signed long  average;
boolean flagReady;

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop()
{
  
  flagReady = false;
  timeCount++;
  sum += int(analogRead(sens));
  if ( timeCount = MEASURE_PERIOD ){
    timeCount= 0;
    average = sum;
    sum = 0;
    flagReady= true;
    if (flagReady && (average/500 > por)){
        average = 0;
        digitalWrite(led, HIGH);
        return;
      }
      else{digitalWrite(led, LOW); return;}  
  }


//  else digitalWrite(led, LOW);

  
//  if (!digitalRead(sens) && !stat && !flag)
//  {
//    flag = true;
//    stat = false;
//  }
//  if (digitalRead(sens) && !stat && !flag)
//  {
//    stat = true;
//    flag = false;
//  }
//
//  if (!digitalRead(sens) && stat && !flag)
//  {
//    delay (10000);
//    if (!digitalRead(sens))
//    {
//      onBat();                                      //Выключаем АКБ
//      stat = false;
//      flag = true;
//    }
//  }
//
//  if (!digitalRead(sens) && !stat && flag)
//  {
//    delay (500);
//    if (digitalRead(sens))
//    {
//      onBat();                                      //Включаем АКБ
//      stat = true;
//      flag = false;
//    }
//    else return;
//  }
//
//

}

//void onBat()
//{
//  digitalWrite(but_tap, LOW);
//  delay(300);
//  digitalWrite(but_tap, HIGH);
//  delay(600);
//  digitalWrite(but_tap, LOW);
//  delay(3000);
//  digitalWrite(but_tap, HIGH);
//  delay(2000);
//}

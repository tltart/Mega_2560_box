#include <ServoSmooth.h>
ServoSmooth servo;

int newPos = 1500;
String str;

signed long time_start;

void setup() {
  Serial.begin(115200);
  servo.attach(12, 1500, 2000);
  servo.setSpeed(10);
  servo.setAccel(0.3);
  servo.setAutoDetach(false);
}

void loop() {

  if (Serial.available() > 0) {
      	str = Serial.readString();
        newPos = str.toInt();
        Serial.println(newPos);
    }
  if (millis() - time_start > 5000){
    Serial.println(servo.getCurrent());
    time_start = millis();
  }
  servo.tick();
  servo.setTarget(newPos);
}

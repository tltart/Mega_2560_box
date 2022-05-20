#include <Servomotor.h>

Servomotor::Servomotor(){}

void Servomotor::Init(ServoSmooth &servo){
  servo.attach(5, 1500, 2000);
  servo.setSpeed(10);
  servo.setAccel(0.3);
  servo.setAutoDetach(false);
}
void Servomotor::Loop(ServoSmooth &servo){
  servo.tick();
}
void Servomotor::ToPosition(ServoSmooth &servo, int newPos){
  // servo.setTarget(newPos);
  //не используется, т.к. не смог передать экземпляр из MqttClient.cpp. Реализация там.
}
void Servomotor::StartPosition(ServoSmooth &servo){
  // servo.setTarget(1500);
  //не используется, т.к. не смог передать экземпляр из MqttClient.cpp. Реализация там.
}

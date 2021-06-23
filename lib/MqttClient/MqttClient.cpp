#include <MqttClient.h>
#include <Servomotor.h>

const char *mqtt_server = "192.168.128.115";

StaticJsonDocument<30> JSON_pult;
StaticJsonDocument<30> JSON_antenn;

char buffer_pult[30];
char buffer_pult_1[30];
int late_state_pult = -1;
char buffer_antenn[40];


// void on_pult()
// {
//   Serial.println("Вызвана функция включения пульта");
//   Serial.println("1 нажатие");
//   digitalWrite(pult, HIGH);
//   delay(500);
//   digitalWrite(pult, LOW);
//   Serial.println("Кнопка отпущена...");
//   delay(400);
//   Serial.println("2 нажатие");
//   digitalWrite(pult, HIGH);
//   delay(3000);
//   digitalWrite(pult, LOW);
//   Serial.println("Отущена после длительного нажатия");
// }
// void off_pult()
// {
//   Serial.println("Вызвана функция выключения пульта");
//   Serial.println("1 нажатие");
//   digitalWrite(pult, HIGH);
//   delay(300);
//   digitalWrite(pult, LOW);
//   Serial.println("Кнопка отпущена...");
//   delay(600);
//   Serial.println("2 нажатие");
//   digitalWrite(pult, HIGH);
//   delay(3000);
//   digitalWrite(pult, LOW);
//   Serial.println("Отущена после длительного нажатия");
// }

ConnectMqtt::ConnectMqtt()
{
}
void ConnectMqtt::InitPin()
{
  pinMode(pult, OUTPUT);
  digitalWrite(pult, LOW);
  pinMode(pult_state, INPUT);

}

void ConnectMqtt::InitMqtt(PubSubClient &client)
{
  void receivedCallback(char *topic, byte *payload, unsigned int length);
  client.setServer(mqtt_server, 1883);
  client.setCallback(receivedCallback);
}

// void receivedCallback(char *topic, byte *payload, unsigned int length)
// {

//   if (String(topic) == POWER_PULT)
//   {
//     for (int i = 0; i < length; i++)
//     {
//       buffer_pult_1[i] = ((char)payload[i]);
//       // Serial.println(String(buffer_pult_1));
//     }

//     if (String(buffer_pult_1) == "ON")
//     {
//       Serial.println("Включение пульта...");
//       on_pult();
//     }
//     else if (String(buffer_pult_1) == "OFF")
//     {
//       Serial.println("Выключение пульта...");
//       off_pult();
//     }
//     else if (String(buffer_pult_1) == "RES")
//     {
//       Serial.println("Перезагрузка пульта...");
//       off_pult();
//       delay(5000);
//       on_pult();
//     }
//     memset(buffer_pult_1, 0, 30);
//   }
//   else if (String(topic) == ANTENN)
//   {
//     for (int i = 0; i < length; i++)
//     {
//       buffer_antenn[i] = ((char)payload[i]);
//     }
//     if (String(buffer_antenn) == "NORTH")
//     {

//     }
//     else if (String(buffer_antenn) == "EAST")
//     {
      
//     }
//     else if (String(buffer_antenn) == "SOUTH")
//     {

//     }
//     else if (String(buffer_antenn) == "WEST")
//     {
      
//     }
//     memset(buffer_antenn, 0, 20);
//   }
// }
void ConnectMqtt::MqttConnect(PubSubClient &client)
{
  /* Loop until reconnected */
  if (!client.connected())
  {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "D1_mini_box";
    const char *sokol = "sokol";
    const char *passw = "9556";

    /* connect now */
    if (client.connect(clientId.c_str(), sokol, passw))
    {
      Serial.println("connected");
      client.subscribe(POWER_PULT);
      client.subscribe(ANTENN);

      /* subscribe topic with default QoS 0*/
    }
  }
}

void ConnectMqtt::SendPultState(PubSubClient &client)
{
  int val_volt = analogRead(pult_state);

  if (val_volt > 200 && late_state_pult != 1)
  {
    JSON_pult["Pult_status"] = "1";
    serializeJsonPretty(JSON_pult, buffer_pult);
    client.publish(PULT_STATE, buffer_pult, true);
    late_state_pult = 1;
    JSON_pult.clear();
  }
  if (val_volt < 50 && late_state_pult != 0)
  {
    JSON_pult["Pult_status"] = "0";
    serializeJsonPretty(JSON_pult, buffer_pult);
    client.publish(PULT_STATE, buffer_pult, true);
    late_state_pult = 0;
    JSON_pult.clear();
  }
}
void ConnectMqtt::SendAntennState(PubSubClient &client, String _send)
{
  JSON_antenn["STATUS"] = _send;
  serializeJsonPretty(JSON_antenn, buffer_antenn);
  client.publish(ANTENN_STATUS, buffer_antenn, true);
  JSON_antenn.clear();
}

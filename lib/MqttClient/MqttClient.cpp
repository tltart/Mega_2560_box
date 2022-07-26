#include <MqttClient.h>

const char *mqtt_server = "192.168.89.115";

StaticJsonDocument<30> JSON_pult;

char buffer_pult[30];
char buffer_pult_1[30];
int late_state_pult = -1;


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

void ConnectMqtt::MqttConnect(PubSubClient &client)
{
  /* Loop until reconnected */
  if (!client.connected())
  {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "D1_mini_box";
    const char *login = "nanopi";
    const char *passw = "9556";

    /* connect now */
    if (client.connect(clientId.c_str(), login, passw))
    {
      Serial.println("connected");
      client.subscribe(POWER_PULT);
    }
  }
}

void ConnectMqtt::SendPultState(PubSubClient &client)
{
  int val_volt = digitalRead(pult_state);

  if (!val_volt && late_state_pult != 1)
  {
    JSON_pult["Pult_status"] = "1";
    serializeJsonPretty(JSON_pult, buffer_pult);
    client.publish(PULT_STATE, buffer_pult, true);
    late_state_pult = 1;
    JSON_pult.clear();
  }
  if (val_volt && late_state_pult != 0)
  {
    JSON_pult["Pult_status"] = "0";
    serializeJsonPretty(JSON_pult, buffer_pult);
    client.publish(PULT_STATE, buffer_pult, true);
    late_state_pult = 0;
    JSON_pult.clear();
  }
}

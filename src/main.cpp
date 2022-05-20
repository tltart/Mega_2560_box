#include <Arduino.h>
#include <DallasTemperature.h>
#include <Ethernet.h>
#include <WebSocketServer.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Drone.h>
#include <MqttClient.h>
#include <Climat.h>
#include <ServoSmooth.h>
#include <Servomotor.h>

using namespace net;

String varr;
String vall;

unsigned long start;
unsigned long start_1;

String JSONtxt;
Drone dron;
ConnectMqtt ConMqtt;
ClimatBox Climat;
Registor reg;

ServoSmooth servo;
Servomotor servomotor;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String clientId = "D1_mini_box";
const char *sokol = "sokol";
const char *passw = "9556";

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress      local_ip   (192,168,128,116);
// IPAddress local_ip(192, 168, 88, 155);

EthernetClient espClient;
PubSubClient client(espClient);
EthernetServer server(80);

constexpr uint16_t port = 3000;
WebSocketServer wss(port);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////----MQTT----////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void receivedCallback(char *topic, byte *payload, unsigned int length);
void on_pult();
void off_pult();
void ToPosition(ServoSmooth &servo, int newPos);
void StartPosition(ServoSmooth &servo);
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void setup()
{
  sensors.begin();
  Serial.begin(115200);
  Serial.println("Setup");

  SPI.begin();

  servomotor.Init(servo);

  dron.pinInit();
  dron.PultInit();

  Climat.InitPin();
  reg.Init();
  ConMqtt.InitPin();

  Ethernet.begin(mac, local_ip);

  server.begin();
  Serial.println(Ethernet.localIP());

  // ConMqtt.InitMqtt(client);
  //////////////////////////////////////////////////////
  /////-----------MQTT-------------------//////////////
  /////////////////////////////////////////////////////
  client.setServer(mqtt_server, 1883);
  client.setCallback(receivedCallback);
  ////////////////////////////////////////////////////
  ////////////////////////////////////////////////////

  wss.onConnection([](WebSocket &ws)
                   {
                     ws.onMessage([](WebSocket &ws, const WebSocket::DataType &dataType, const char *message, uint16_t length)
                                  {
                                    String payloadString = message;
                                    // Serial.print("payloadString= ");
                                    switch (dataType)
                                    {
                                    case WebSocket::DataType::TEXT:
                                      byte separator = payloadString.indexOf('=');
                                      varr = payloadString.substring(0, separator);
                                      vall = payloadString.substring(separator + 1);
                                      if (varr == "pult")
                                      {
                                        if (vall == "1")
                                        {
                                          reg.L1();
                                          Serial.println("L1");
                                        }
                                        if (vall == "2")
                                        {
                                          reg.L2();
                                          Serial.println("L2");
                                        }
                                        if (vall == "h")
                                        {
                                          reg.PultHome();
                                          Serial.println("Home!");
                                        }
                                        if (vall == "up")
                                        {
                                          dron.Pitch(throttle_160);
                                          Serial.println("ВПЕРЕД!");
                                        }
                                        if (vall == "fast_forward")
                                        {
                                          dron.Pitch(throttle_190);
                                          Serial.println("Быстро вперед!");
                                        }
                                        if (vall == "down")
                                        {
                                          dron.Pitch(throttle_90);
                                          Serial.println("НАЗАД!");
                                        }
                                        if (vall == "fast_back")
                                        {
                                          dron.Pitch(throttle_60);
                                          Serial.println("Быстро назад!");
                                        }
                                        if (vall == "left")
                                        {
                                          dron.Roll(throttle_160);
                                          Serial.println("Лево!");
                                        }
                                        if (vall == "fast_left")
                                        {
                                          dron.Roll(throttle_190);
                                          Serial.println("Быстро влево!");
                                        }
                                        if (vall == "right")
                                        {
                                          dron.Roll(throttle_90);
                                          Serial.println("Право!");
                                        }
                                        if (vall == "fast_right")
                                        {
                                          dron.Roll(throttle_60);
                                          Serial.println("Быстро вправо!");
                                        }
                                        if (vall == "w")
                                        {
                                          dron.Throttle(throttle_150);
                                          Serial.println("Вверх!");
                                        }
                                        if (vall == "fast_up")
                                        {
                                          dron.Throttle(throttle_190);
                                          Serial.println("Быстро вверх!");
                                        }
                                        if (vall == "s")
                                        {
                                          dron.Throttle(throttle_80);
                                          Serial.println("Вниз!");
                                        }
                                        if (vall == "fast_down")
                                        {
                                          dron.Throttle(throttle_25);
                                          Serial.println("Быстро вниз!");
                                        }
                                        if (vall == "a")
                                        {
                                          dron.Yaw(throttle_80);
                                          Serial.println("Ось влево!");
                                        }
                                        if (vall == "d")
                                        {
                                          dron.Yaw(throttle_180);
                                          Serial.println("Ось вправо!");
                                        }
                                        if (vall == "r")
                                        {
                                          dron.Gimball(throttle_190);
                                          Serial.println("Подвес вверх!");
                                        }
                                        if (vall == "f")
                                        {
                                          dron.Gimball(throttle_60);
                                          Serial.println("Подвес вниз!");
                                        }
                                        if (vall == "+")
                                        {
                                          dron.GimballZoom(throttle_190);
                                          Serial.println("Зум + !");
                                        }
                                        if (vall == "-")
                                        {
                                          dron.GimballZoom(throttle_60);
                                          Serial.println("Зум - !");
                                        }
                                        if (vall == "@")
                                        {
                                          dron.PultInit();
                                          Serial.println("Cтоп!");
                                        }
                                      }
                                    }
                                  });

                     ws.onClose(
                         [](WebSocket &ws, const WebSocket::CloseCode &code, const char *reason,
                            uint16_t length)
                         { Serial.println(F("Disconnected")); });

                     Serial.print(F("New client: "));
                     Serial.println(ws.getRemoteIP());

                     const char message[]{"Hello from Arduino server!"};
                     ws.send(WebSocket::DataType::TEXT, message, strlen(message));
                   });

  wss.begin();

  client.setServer(mqtt_server, 1883);
  client.setCallback(receivedCallback);
}

void loop()
{

  while (!client.connected())
  {
    ConMqtt.MqttConnect(client);
    sensors.requestTemperatures();
    Climat.TecPower_out_Mqtt(sensors, reg);
    Climat.FanSpeed_out_Mqtt(sensors, reg);
  }

  wss.listen();
  client.loop();

  if (millis() - start > 10000)
  {
    start = millis();
    sensors.requestTemperatures();
    Climat.TecPower(sensors, client, reg);
    Climat.FanSpeed(sensors, client, reg);
  }

  ConMqtt.SendPultState(client);
  servomotor.Loop(servo);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/////////////----MQTT----//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void receivedCallback(char *topic, byte *payload, unsigned int length)
{

  if (String(topic) == POWER_PULT)
  {
    for (int i = 0; i < length; i++)
    {
      buffer_pult_1[i] = ((char)payload[i]);
      // Serial.println(String(buffer_pult_1));
    }

    if (String(buffer_pult_1) == "ON")
    {
      // Serial.println("Включение пульта...");
      on_pult();
    }
    else if (String(buffer_pult_1) == "OFF")
    {
      // Serial.println("Выключение пульта...");
      off_pult();
    }
    else if (String(buffer_pult_1) == "RES")
    {
      // Serial.println("Перезагрузка пульта...");
      off_pult();
      delay(5000);
      on_pult();
    }
    memset(buffer_pult_1, 0, 30);
  }
  else if (String(topic) == ANTENN)
  {
    for (int i = 0; i < length; i++)
    {
      buffer_antenn[i] = ((char)payload[i]);
    }
    if (String(buffer_antenn) == "N")
    {
      ToPosition(servo, 1850);
    }
    if (String(buffer_antenn) == "NE")
    {
      ToPosition(servo, 1975);
    }
    if (String(buffer_antenn) == "NW")
    {
      ToPosition(servo, 1675);
    }
    else if (String(buffer_antenn) == "E")
    {
      ToPosition(servo, 2100);
    }
    else if (String(buffer_antenn) == "S")
    {
      ToPosition(servo, 1150);
    }
    else if (String(buffer_antenn) == "SE")
    {
      ToPosition(servo, 900);
    }
    else if (String(buffer_antenn) == "SW")
    {
      ToPosition(servo, 1325);
    }
    else if (String(buffer_antenn) == "W")
    {
      ToPosition(servo, 1500);
    }
    ConMqtt.SendAntennState(client, String(buffer_antenn));
    memset(buffer_antenn, 0, 40);
  }
}

void ToPosition(ServoSmooth &servo, int newPos)
{
  servo.setTarget(newPos);
};
void StartPosition(ServoSmooth &servo)
{
  servo.setTarget(1500);
};
void off_pult()
{
  Serial.println("Вызвана функция выключения пульта");
  Serial.println("1 нажатие");
  digitalWrite(pult, HIGH);
  delay(300);
  digitalWrite(pult, LOW);
  Serial.println("Кнопка отпущена...");
  delay(600);
  Serial.println("2 нажатие");
  digitalWrite(pult, HIGH);
  delay(3000);
  digitalWrite(pult, LOW);
  Serial.println("Отущена после длительного нажатия");
}
void on_pult()
{
  Serial.println("Вызвана функция включения пульта");
  Serial.println("1 нажатие");
  digitalWrite(pult, HIGH);
  delay(500);
  digitalWrite(pult, LOW);
  Serial.println("Кнопка отпущена...");
  delay(400);
  Serial.println("2 нажатие");
  digitalWrite(pult, HIGH);
  delay(3000);
  digitalWrite(pult, LOW);
  Serial.println("Отущена после длительного нажатия");
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
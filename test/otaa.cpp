#include <Arduino.h>

#include <DallasTemperature.h>

#include <Ethernet.h>
#include <WebSocketServer.h>

#include <PubSubClient.h>
#include <SPI.h>

#include <Drone.h>
#include <MqttClient.h>
#include <Climat.h>

using namespace net;

String varr;
String vall;

unsigned long start;

String              JSONtxt;
Drone               dron;
ConnectMqtt         ConMqtt;
ClimatBox           Climat;
Registor            reg;

OneWire             oneWire(ONE_WIRE_BUS);
DallasTemperature   sensors(&oneWire);

// char buffer_pult[10];
byte           mac[] =    { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress      local_ip   (192,168,88,116);
// IPAddress   gateway    (192,168,88,1);
// IPAddress   subnet     (255,255,255,0);

EthernetClient          espClient;
PubSubClient            client(espClient);
EthernetServer          server(80);

constexpr uint16_t port = 3000;
WebSocketServer wss(port);

void setup() {
    sensors.begin();
    Serial.begin(115200);
    Serial.println("Setup");

    SPI.begin();

    dron.pinInit();  
    dron.PultInit();

    Climat.InitPin();
    reg.Init();
    ConMqtt.InitPin();

    Ethernet.begin(mac, local_ip);

    server.begin();
    Serial.println(Ethernet.localIP());

    ConMqtt.InitMqtt(client);

    wss.onConnection([](WebSocket &ws) 
    {
      ws.onMessage([](WebSocket &ws, const WebSocket::DataType &dataType, const char *message, uint16_t length) 
        {
          String payloadString = message;
          Serial.print("payloadString= ");
          // Serial.println(payloadString);
          // Serial.println(dataType);
          switch (dataType) {
            case WebSocket::DataType::TEXT:
            // Serial.print(F("Received: "));
            byte separator=payloadString.indexOf('=');
            varr = payloadString.substring(0,separator);
            vall = payloadString.substring(separator+1);
            // Serial.print("var= ");
            // Serial.println(varr);
            
            // Serial.print("val= ");
            // Serial.print(vall);

            if (varr == "pult")
          { 
            if(vall == "up")
            {
              dron.Pitch(throttle_160);
              Serial.println("ВПЕРЕД!");
            }
            if(vall=="fast_forward")
            {
              dron.Pitch(throttle_190);
              Serial.println("Быстро вперед!");
            }
            if(vall=="down")
            {
              dron.Pitch(throttle_90);
              Serial.println("НАЗАД!");
            }
            if(vall=="fast_back")
            {
              dron.Pitch(throttle_60);
              Serial.println("Быстро назад!");
            }
            if(vall=="left")
            {
              dron.Roll(throttle_160);
              Serial.println("Лево!");
            }
            if(vall=="fast_left")
            {
              dron.Roll(throttle_190);
              Serial.println("Быстро влево!");
            }
            if(vall=="right")
            {
              dron.Roll(throttle_90);
              Serial.println("Право!");
            }
            if(vall=="fast_right")
            {
              dron.Roll(throttle_60);
              Serial.println("Быстро вправо!");
            }
            if(vall=="w")
            {
              dron.Throttle(throttle_150);
              Serial.println("Вверх!");
            }
            if(vall=="fast_up")
            {
              dron.Throttle(throttle_190);
              Serial.println("Быстро вверх!");
            }     
            if(vall=="s")
            {
              dron.Throttle(throttle_80);
              Serial.println("Вниз!");
            }
            if(vall=="fast_down")
            {
              dron.Throttle(throttle_25);
              Serial.println("Быстро вниз!");
            }           
            if(vall=="a")
            {
              dron.Yaw(throttle_80);
              Serial.println("Ось влево!");
            }
            if(vall=="d")
            {
              dron.Yaw(throttle_180);
              Serial.println("Ось вправо!");
            }
            if(vall=="r")
            {
              dron.Gimball(throttle_190);
              Serial.println("Подвес вверх!");
            }
            if(vall=="f")
            {
              dron.Gimball(throttle_60);
              Serial.println("Подвес вниз!");
            }
            if(vall=="+")
            {
              dron.GimballZoom(throttle_190);
              Serial.println("Зум + !");
            }
            if(vall=="-")
            {
              dron.GimballZoom(throttle_60);
              Serial.println("Зум - !");
            }
            if(vall=="@")
            {
              dron.PultInit();
              Serial.println("Cтоп!");
            }
          }    
  
          // Serial.println(message);
          // break;
          // case WebSocket::DataType::BINARY:
          //   Serial.println(F("Received binary data"));
          //   break;
        }
  
        // ws.send(dataType, message, length);
    });

    ws.onClose(
      [](WebSocket &ws, const WebSocket::CloseCode &code, const char *reason,
        uint16_t length) { Serial.println(F("Disconnected")); });

    Serial.print(F("New client: "));
    Serial.println(ws.getRemoteIP());

    const char message[]{ "Hello from Arduino server!" };
    ws.send(WebSocket::DataType::TEXT, message, strlen(message));
  });

  wss.begin();

}



void loop() {

    wss.listen();

    if (!client.connected()) 
      { 
        ConMqtt.MqttConnect(client);
      } 

    client.loop();


    if (millis() - start > 10000)
    {
      start = millis();
      sensors.requestTemperatures();
      Climat.TecPower(sensors, client, reg);
      Climat.FanSpeed(sensors, client, reg);
    }

    ConMqtt.SendPultState(client);

}
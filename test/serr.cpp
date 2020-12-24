#include <Ethernet.h>

#include <WebSocketServer.h>
#include <SPI.h>
using namespace net;


byte mac[]{ 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 88, 116);

constexpr uint16_t port = 88;
WebSocketServer ws(port);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  // Ethernet.init(53);
  Ethernet.begin(mac, ip);

  
  Serial.print(F("Server running at "));
  Serial.print(Ethernet.localIP());
  Serial.print(F(":"));
  Serial.println(port);


  ws.onConnection([](WebSocket &ws) {
    Serial.println("Connet new client");
    const char message[]{ "Hello from Arduino server!" };
    ws.send(WebSocket::DataType::TEXT, message, strlen(message));
    ws.onMessage([](WebSocket &ws, const WebSocket::DataType &dataType, const char *message, uint16_t length) 
      {
        switch (dataType) {
        case WebSocket::DataType::TEXT:
          Serial.print(F("Received: "));
          Serial.println(message);
          break;
        case WebSocket::DataType::BINARY:
          Serial.println(F("Received binary data"));
          break;
      }

      // ws.send(message, length);
      });

    ws.onClose(
      [](WebSocket &ws, const WebSocket::CloseCode &code, const char *reason,
        uint16_t length) { Serial.println(F("Disconnected")); });

    Serial.print(F("New client: "));
    Serial.println(ws.getRemoteIP());


  });

  ws.begin();

}


uint32_t previousTime = 0;

void loop() { 

  ws.listen(); 
  
  }
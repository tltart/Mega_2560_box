#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Servomotor.h>
#include <ServoSmooth.h>


#define pult                    A1   // A1
#define pult_state              A0   // A0

extern int late_state_pult;


#define PULT_STATE        "box/pult_status"
#define POWER_PULT        "power/pult"
#define ANTENN            "antenn/direction"
#define ANTENN_STATUS     "antenn/status"
                            
extern const char* mqtt_server;
extern char buffer_pult[30];
extern char buffer_pult_1[30];

extern char buffer_antenn[40];

extern StaticJsonDocument<30> JSON_pult;
extern StaticJsonDocument<30> JSON_antenn;

class ConnectMqtt
{
    private:

    public:

        ConnectMqtt();
        void InitPin();
        void InitMqtt(PubSubClient &client);
        void MqttConnect(PubSubClient &client);
        void SendPultState(PubSubClient &client);
        void SendAntennState(PubSubClient &client, String _send);
        void AntennDirection(ServoSmooth &servo);
};



#endif
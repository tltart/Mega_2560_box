#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>


#define pult                    13   // A1
#define pult_state              15   // A0

extern int late_state_pult;


#define PULT_STATE        "box/pult_status"
#define POWER_PULT        "power/pult"
                            
extern const char* mqtt_server;
extern char buffer_pult[30];
extern char buffer_pult_1[30];

extern StaticJsonDocument<30> JSON_pult;

class ConnectMqtt
{
    private:

    public:

        ConnectMqtt();
        void InitPin();
        void InitMqtt(PubSubClient &client);
        void MqttConnect(PubSubClient &client);
        void SendPultState(PubSubClient &client);
};



#endif
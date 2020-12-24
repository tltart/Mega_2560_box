#ifndef CLIMAT_H
#define CLIMAT_H

// #include <iostream>
#include <Arduino.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <MqttClient.h>
#include <Registor.h>

#define ONE_WIRE_BUS 11  // D11

#define FAN          3   // D3

#define TEMP_TOPIC        "box/temp/tec"
#define TEMP_TEC_STATUS   "box/temp/tec/status"
#define TEMP_TOPIC_2      "box/temp/fan"



extern int fan_if_else;

extern int  sen_2_on;                                                            // порог включения пельтье на охлаждение
extern int  sen_2_off;                                                           // выключение пельтье на охлаждение
extern int  sen_2_heat_on;                                                       // включение пельтье на обогрев;
extern int  sen_2_heat_off;                                                      // выключение обогрева пельтье;

extern uint8_t sensor1[8];
extern uint8_t sensor2[8];
// extern uint8_t sensor2[8];


extern StaticJsonDocument<100> JSONtxtt;
extern char buffer[100];


extern byte type_tec;


extern int sen_1_high;                                                          // вентилятора на 100%
extern int sen_1_low;                                                           // вентилятор 40%

extern int tempC_1;
extern int tempC_1_last;
extern int tempC_2;
extern int tempC_2_last;
extern int TEC_work;
extern int TEC_work_last;
extern int FAN_speed;
extern int FAN_speed_last;


class ClimatBox
{
    private:

    public:
        ClimatBox();
        void InitPin();
        void TecPower_out_Mqtt(DallasTemperature &sensors, Registor &reg);
        void FanSpeed_out_Mqtt(DallasTemperature &sensors, Registor &reg);
        void TecPower(DallasTemperature &sensors, PubSubClient &client, Registor &reg);
        void FanSpeed(DallasTemperature &sensors, PubSubClient &client, Registor &reg);
};




#endif
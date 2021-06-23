#include <Climat.h>




int fan_if_else = -1;

uint8_t sensor2[8] = { 0x28, 0xFF, 0x25, 0x46, 0xA1, 0x15, 0x03, 0x27 };
uint8_t sensor1[8] = { 0x28, 0xFF, 0x65, 0x60, 0xA1, 0x15, 0x04, 0x34 };

StaticJsonDocument<100> JSONtxtt;
char buffer[100];


int sen_2_on        = 20;       // порог включения пельтье на охлаждение
int sen_2_off       = 15;       // выключение пельтье на охлаждение
int sen_2_heat_on   = 10;       // включение пельтье на обогрев;
int sen_2_heat_off  = 15;       // выключение обогрева пельтье;
byte type_tec;                  // режим работы пельтье (1/2 холод/тепло)


int sen_1_high      = 50;       // вентилятора на 100%
int sen_1_low       = 30;       // вентилятор 40%

int tempC_1;                    // температура наружнего радиатора
int tempC_1_last;               
int tempC_2;
int tempC_2_last;
int TEC_work;
int TEC_work_last   = -1;
int FAN_speed;
int FAN_speed_last  = -1;


ClimatBox::ClimatBox()
{
    
}

void ClimatBox::InitPin()
{
  pinMode(FAN, OUTPUT);
  analogWrite(FAN, 0);
}
void ClimatBox::TecPower_out_Mqtt(DallasTemperature &sensors, Registor &reg){

  tempC_2 = sensors.getTempC(sensor2);
  
    if (tempC_2 > sen_2_on && type_tec != 1) 
    {
      reg.TecOn(1);
      TEC_work = 100;
      type_tec = 1;
      Serial.println("ТЕК холод");
    }
                           
    else if(tempC_2 < sen_2_off && type_tec != 0 && type_tec != 2)
      {        
        reg.TecOn(0);
        TEC_work = 0;
        type_tec = 0;
        Serial.println("ТЕК не работает");
      }

    else if (tempC_2 < sen_2_heat_on && type_tec != 2)
      {
        reg.TecOn(2);
        TEC_work = 100;
        type_tec = 2;
        Serial.println("ТЕК тепло");
      }
      
    else if(tempC_2 > sen_2_heat_off && type_tec != 0 && type_tec != 1)
      {
        reg.TecOn(0);
        TEC_work = 0;
        type_tec = 0;
        Serial.println("ТЕК не работает");
      }
}
void ClimatBox::FanSpeed_out_Mqtt(DallasTemperature &sensors, Registor &reg){
    tempC_1 = sensors.getTempC(sensor1);

    if(tempC_1 > sen_1_high)
    { 
      if (fan_if_else == 0 || fan_if_else == 2 || fan_if_else == 1 || fan_if_else == -1)
      {
        analogWrite(FAN, 1023);
        FAN_speed = 100;
        fan_if_else = 3;
        Serial.println("FAN работает 100%");
      }  
    }
    else if((sen_1_low - 10) < tempC_1 && tempC_1 < sen_1_high)
    {
      if (fan_if_else == 1 || fan_if_else == 0 || fan_if_else == 3 || fan_if_else == -1)
      {
        analogWrite(FAN, 400);
        FAN_speed = 40;
        fan_if_else = 2;
        Serial.println("FAN работает 40%");
      }
    }  
    else if(tempC_1 < 15)
    {
      if (fan_if_else == 1 || fan_if_else == 2 || fan_if_else == 3|| fan_if_else == -1)
      {
        analogWrite(FAN, 0);
        FAN_speed = 0;
        Serial.println("FAN не работает");
        fan_if_else = 0;
      }  
    }
}
void ClimatBox::TecPower(DallasTemperature &sensors, PubSubClient &client, Registor &reg){
    tempC_2 = sensors.getTempC(sensor2);
  
    if (tempC_2 > sen_2_on && type_tec != 1) 
    {
      reg.TecOn(1);
      TEC_work = 100;
      type_tec = 1;
      Serial.println("ТЕК холод");
    }
                           
    else if(tempC_2 < sen_2_off && type_tec != 0 && type_tec != 2)
      {        
        reg.TecOn(0);
        TEC_work = 0;
        type_tec = 0;
        Serial.println("ТЕК не работает");
      }

    else if (tempC_2 < sen_2_heat_on && type_tec != 2)
      {
        reg.TecOn(2);
        TEC_work = 100;
        type_tec = 2;
        Serial.println("ТЕК тепло");
      }
      
    else if(tempC_2 > sen_2_heat_off && type_tec != 0 && type_tec != 1)
      {
        reg.TecOn(0);
        TEC_work = 0;
        type_tec = 0;
        Serial.println("ТЕК не работает");
      }

    if (tempC_2 != tempC_2_last){
        JSONtxtt["box_temp_in"] = tempC_2;
        serializeJsonPretty(JSONtxtt, buffer);
        Serial.println(buffer);
        client.publish(TEMP_TOPIC, buffer, true);
        tempC_2_last = tempC_2;
        JSONtxtt.clear();
    }
    if (TEC_work_last != TEC_work){
        JSONtxtt["TEC_work"] = TEC_work;
        JSONtxtt["TEC_type"] = type_tec;
        serializeJsonPretty(JSONtxtt, buffer);
        Serial.println(buffer);
        client.publish(TEMP_TEC_STATUS, buffer, true);
        TEC_work_last = TEC_work;
        JSONtxtt.clear();
      }
    
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    
//================================================ Работа вентилятора ===================================================//

void ClimatBox::FanSpeed(DallasTemperature &sensors, PubSubClient &client, Registor &reg){
    tempC_1 = sensors.getTempC(sensor1);

    if(tempC_1 > sen_1_high)
    { 
      if (fan_if_else == 0 || fan_if_else == 2 || fan_if_else == 1 || fan_if_else == -1)
      {
        analogWrite(FAN, 1023);
        FAN_speed = 100;
        fan_if_else = 3;
        Serial.println("FAN работает 100%");
      }  
    }
    else if((sen_1_low - 10) < tempC_1 && tempC_1 < sen_1_high)
    {
      if (fan_if_else == 1 || fan_if_else == 0 || fan_if_else == 3 || fan_if_else == -1)
      {
        analogWrite(FAN, 400);
        FAN_speed = 40;
        fan_if_else = 2;
        Serial.println("FAN работает 40%");
      }
    }  
    else if(tempC_1 < 15)
    {
      if (fan_if_else == 1 || fan_if_else == 2 || fan_if_else == 3|| fan_if_else == -1)
      {
        analogWrite(FAN, 0);
        FAN_speed = 0;
        Serial.println("FAN не работает");
        fan_if_else = 0;
      }  
    }

    if (tempC_1 != tempC_1_last){
      JSONtxtt["box_temp_out"] = tempC_1;
      serializeJsonPretty(JSONtxtt, buffer);
      Serial.println(buffer);
      client.publish(TEMP_TOPIC_2, buffer, true);
      tempC_1_last = tempC_1;
      JSONtxtt.clear();
    }
    if (FAN_speed_last != FAN_speed){
      JSONtxtt["FAN_speed"] = FAN_speed;
      serializeJsonPretty(JSONtxtt, buffer);
      Serial.println(buffer);
      client.publish(TEMP_TOPIC_2, buffer, true);
      FAN_speed_last = FAN_speed;
      JSONtxtt.clear();
      }
   
    
  }
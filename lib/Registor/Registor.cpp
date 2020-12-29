#include <Registor.h>

int tec_mode = -1;

unsigned long start_home;

byte buffer_registr = 0b00111000;

byte off_all        = 0b00111000;
byte tec_mode_cold  = 0b11000000;
byte tec_mode_heat  = 0b10000000;
byte pult_home_pin  = 0b00100000;
byte pult_L1_pin    = 0b00010000;
byte pult_L2_pin    = 0b00001000;


Registor::Registor()
{

}

void Registor::Init()
{
    pinMode(clatch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);
    digitalWrite(clatch_pin, HIGH);
    digitalWrite(clatch_pin, LOW);                                     
    shiftOut(data_pin, clock_pin, LSBFIRST, off_all); 
    digitalWrite(clatch_pin, HIGH);
}

void Registor::TecOn(int tec_mode)
{   
    if (tec_mode == 1)
    {        
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, tec_mode_cold); 
        digitalWrite(clatch_pin, HIGH);
        buffer_registr = tec_mode_cold;
        // Serial.println("TEC холод");
        // Serial.println(buffer_registr);
        
    }
    if (tec_mode == 2)
    {
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, tec_mode_heat); 
        digitalWrite(clatch_pin, HIGH);
        buffer_registr = tec_mode_heat;
        Serial.println("TEC тепло");
        // Serial.println(buffer_registr);
    }
    if (tec_mode == 0)
    {
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, off_all); 
        digitalWrite(clatch_pin, HIGH);
        buffer_registr = off_all;
        Serial.println("TEC не работает");
        // Serial.println(buffer_registr);
    }
}

void Registor::PultHome()
{       
        // Serial.println("Нажали ДОМОЙ");
        buffer_registr ^= pult_home_pin;
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, buffer_registr); 
        digitalWrite(clatch_pin, HIGH);
        delay(3000);
        buffer_registr |= pult_home_pin;
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, buffer_registr); 
        digitalWrite(clatch_pin, HIGH);
        Serial.println("Отпустили ДОМОЙ");

}

void Registor::L1()
{
        Serial.println("Нажали L1");
        buffer_registr ^= pult_L1_pin;
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, buffer_registr); 
        digitalWrite(clatch_pin, HIGH);
        delay(500);
        buffer_registr |= pult_L1_pin;
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, buffer_registr); 
        digitalWrite(clatch_pin, HIGH);
        Serial.println("Отпустили L1");
}

void Registor::L2()
{
        Serial.println("Нажали L2");
        buffer_registr ^= pult_L2_pin;
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, buffer_registr); 
        digitalWrite(clatch_pin, HIGH);
        delay(500);
        buffer_registr |= pult_L2_pin;
        digitalWrite(clatch_pin, LOW);                                     
        shiftOut(data_pin, clock_pin, LSBFIRST, buffer_registr); 
        digitalWrite(clatch_pin, HIGH);
        Serial.println("Отпустили L2");
}


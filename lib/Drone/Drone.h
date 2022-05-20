#ifndef DRONE_H
#define DRONE_H
#include <Arduino.h>


#define CS   7     // D7                     //// chip select
#define clck 4     // D4                     //// clock
#define dd   9     // D9                     //// data


extern byte resis_1;                //// address first potenciometr
extern byte resis_2;                //// address second potenciometr



extern byte throttle_25; 
extern byte throttle_50; 
extern byte throttle_60; 
extern byte throttle_70; 
extern byte throttle_80; 
extern byte throttle_90; 
extern byte throttle_100;
extern byte throttle_110;
extern byte throttle_120;
extern byte throttle_130;
extern byte throttle_140;
extern byte throttle_150;
extern byte throttle_160;
extern byte throttle_170;
extern byte throttle_180;
extern byte throttle_190;
extern byte throttle_200;

 


class Drone
{
    private:
        
    public:
        Drone();
        void pinInit();
        void Yaw(byte val);
        void Roll(byte val);
        void Pitch(byte val);
        void Throttle(byte val);
        void Gimball(byte val);
        void GimballZoom(byte val);
        void PultInit();
};


#endif
#include <Drone.h>
#include <SPI.h>

byte resis_1 = 0b00010001;
byte resis_2 = 0b00010010;
///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////====Газ====//===Ось===//=Наклон=//=Тангаж=//=Подвес==////////////
//////////////////////////////////////==Throttle=//===Yaw===//==Roll==//=Pitch==//=Gimball=//==Zoom==//
byte throttle_25  = 0b00011001;     //   -100%   //   +100  //  +100  //  -100  //         //        // 
byte throttle_50  = 0b00110010;     //   -81%    //   +100  //  +97   //  -100  //         //        // 
byte throttle_60  = 0b00111100;     //   -66%    //   +87   //  +83   //  -85   //   down  //   -    // 
byte throttle_70  = 0b01000110;     //   -52%    //   +72   //  +69   //  -69   //         //        // 
byte throttle_80  = 0b01010000;     //   -38%    //   +58   //  +55   //  -54   //         //        // 
byte throttle_90  = 0b01011010;     //   -23%    //   +44   //  +41   //  -39   //         //        // 
byte throttle_100 = 0b01100100;     //   -9%     //   +29   //  +26   //  -23   //         //        // 
byte throttle_110 = 0b01101110;     //    0%     //   +15   //  +12   //  -8    //         //        // 
byte throttle_120 = 0b01111000;     //    0%     //   +1    //   0    //   0    //         //        // 
byte throttle_130 = 0b10000010;     //   +10%    //    0    //   0    //   0    //    0    //    0   // 
byte throttle_140 = 0b10001100;     //   +24%    //   -4    //  -7    //  +11   //         //        // 
byte throttle_150 = 0b10010110;     //   +38%    //   -19   //  -22   //  +25   //         //        // 
byte throttle_160 = 0b10100000;     //   +52%    //   -33   //  -38   //  +39   //         //        // 
byte throttle_170 = 0b10101010;     //   +66%    //   -48   //  -54   //  +54   //         //        // 
byte throttle_180 = 0b10110100;     //   +80%    //   -63   //  -70   //  +68   //         //        // 
byte throttle_190 = 0b10111110;     //   +95%    //   -78   //  -85   //  +82   //    up   //    +   // 
byte throttle_200 = 0b11001000;     //   +100%   //   -93   //  -100  //  +96   //         //        // 
///////////////////////////////////////////////////////////////////////////////////////////////////////      


Drone::Drone()
{
}
void Drone::pinInit()
{
        pinMode(CS, OUTPUT);
        pinMode(clck, OUTPUT);
        pinMode(dd, OUTPUT);
        digitalWrite(CS, HIGH); 
}
void Drone::Throttle(byte val) { 
        digitalWrite(CS, LOW);
        shiftOut(dd, clck, MSBFIRST, resis_1);
        shiftOut(dd, clck, MSBFIRST, val);
        digitalWrite(CS,HIGH);
}

void Drone::Yaw(byte val) {       
        digitalWrite(CS, LOW); 
        shiftOut(dd, clck, MSBFIRST, resis_2);
        shiftOut(dd, clck, MSBFIRST, val);    
        digitalWrite(CS,HIGH); 
}

void Drone::Roll(byte val) {                                                                                      
        digitalWrite(CS, LOW);                                                                                                      
        shiftOut(dd, clck, MSBFIRST, resis_1);
        shiftOut(dd, clck, MSBFIRST, val);
        shiftOut(dd, clck, MSBFIRST, resis_1);
        shiftOut(dd, clck, MSBFIRST, throttle_110);
        digitalWrite(CS,HIGH);                                                                                                      
}

void Drone::Pitch(byte val) {                                                                                      
        digitalWrite(CS, LOW);                                                                                                      
        shiftOut(dd, clck, MSBFIRST, resis_2);
        shiftOut(dd, clck, MSBFIRST, val);                                                                          
        shiftOut(dd, clck, MSBFIRST, resis_2);
        shiftOut(dd, clck, MSBFIRST, throttle_130);
        digitalWrite(CS,HIGH);                                                                                                      
}

void Drone::Gimball(byte val){
        digitalWrite(CS, LOW);
        shiftOut(dd, clck, MSBFIRST, resis_1);
        shiftOut(dd, clck, MSBFIRST, val);
        shiftOut(dd, clck, MSBFIRST, resis_1);
        shiftOut(dd, clck, MSBFIRST, throttle_120);
        shiftOut(dd, clck, MSBFIRST, resis_1);
        shiftOut(dd, clck, MSBFIRST, throttle_120);
        digitalWrite(CS,HIGH);
}

void Drone::GimballZoom(byte val){
        digitalWrite(CS, LOW);                                                                                                      
        shiftOut(dd, clck, MSBFIRST, resis_2);
        shiftOut(dd, clck, MSBFIRST, val);                                                                          
        shiftOut(dd, clck, MSBFIRST, resis_2);
        shiftOut(dd, clck, MSBFIRST, throttle_120);
        shiftOut(dd, clck, MSBFIRST, resis_2);
        shiftOut(dd, clck, MSBFIRST, throttle_130);
        digitalWrite(CS,HIGH);
}

void Drone::PultInit(){
    Gimball(throttle_130);
    GimballZoom(throttle_130);
}
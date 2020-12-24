#ifndef REGISTOR_H
#define REGISTOR_H

// #include <iostream>
#include <Arduino.h>
#include <SPI.h>

#define data_pin   2    //  D2
#define clatch_pin 4    //  D4
#define clock_pin  5    //  D5


extern int tec_mode;

extern byte buffer_registr; 

extern byte off_all;
extern byte tec_mode_cold;
extern byte tec_mode_heat;
extern byte pult_home_pin;
extern byte pult_L1_pin;
extern byte pult_L2_pin;

extern int tec_mode;


class Registor
{
    private:

    public:
        Registor();
        void Init();
        void TecOn(int tec_mode);
        void PultHome();
        void L1();
        void L2();


};


#endif
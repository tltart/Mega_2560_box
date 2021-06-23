#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H
#include <Arduino.h>
#include <ServoSmooth.h>

class Servomotor
{
    private:

    public:
        Servomotor();
        void Init(ServoSmooth &servo);
        void Loop(ServoSmooth &servo);
        void ToPosition(ServoSmooth &servo, int newPos);
        void StartPosition(ServoSmooth &servo);
};


#endif
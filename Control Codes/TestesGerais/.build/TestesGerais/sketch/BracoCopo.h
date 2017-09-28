#ifndef Garra_h
#define Garra_h

#include "Arduino.h"
#include <Servo.h>
#include "SharpIR.h"
#include "variables.cpp"

class BracoCopo{
public:
    BracoCopo();
    BracoCopo(int servoPulso, int servoGarra, int sharpGarra);

    int getSharp(int porta);
    
private:
    Servo pulso;
    Servo garra;
    int sharpGarra;
    int motorPasso;
};

#endif
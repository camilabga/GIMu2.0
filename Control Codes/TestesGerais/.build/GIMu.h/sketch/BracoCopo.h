#ifndef Garra_h
#define Garra_h

#include "Arduino.h"
#include <Servo.h>
#include "Stepper_M.h"
#include "SharpIR.h"
#include "variables.cpp"

class BracoCopo{
public:
    BracoCopo();
    BracoCopo(int servoPulso, int servoGarra, int sharpGarra);

    int getSharp(int porta);

    inline Servo getPulso(){return pulso;}
    inline Servo getGarra(){return garra;}
    inline int getSharpGarra(){return sharpGarra;}
    inline Stepper_M getMotorPasso(){return motorPasso;}

    inline void setPulso(Servo pulso){this->pulso = pulso;}
    inline void setGarra(Servo garra){this->garra = garra;}
    inline void setSharpGarra(int sharpGarra){this->sharpGarra = sharpGarra;}
    inline void setMotorPasso(Stepper_M motorPasso){this->motorPasso = motorPasso;}    
    
private:
    Servo pulso;
    Servo garra;
    int sharpGarra;
    Stepper_M motorPasso;
};

#endif
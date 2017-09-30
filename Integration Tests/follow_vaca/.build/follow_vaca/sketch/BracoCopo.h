#ifndef Garra_h
#define Garra_h

#include "Arduino.h"
#include <Servo.h>
#include "SharpIR.h"
#include "Motor.h"
#include "variables.cpp"

class BracoCopo{
public:
    BracoCopo();
    BracoCopo(int servoPulso, int servoGarra, int sharpGarra, Motor m);

    int getSharp();

    inline void setSharpGarra(int sharpGarra){this->sharpGarra = sharpGarra;}
    inline void attachPulso(int porta){pulso.attach(porta);}
    inline void attachGarra(int porta){garra.attach(porta);}
    void attachMotor(Motor m);

    inline int getSharpGarra(){return sharpGarra;}
    inline Motor getMotor(){return motorBraco;}
    inline Servo getPulso(){return pulso;}
    inline Servo getGarra(){return garra;}
    
private:
    Servo pulso;
    Servo garra;
    int sharpGarra;
    Motor motorBraco;
};

#endif
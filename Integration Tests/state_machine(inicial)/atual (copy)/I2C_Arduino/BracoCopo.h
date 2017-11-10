#ifndef Garra_h
#define Garra_h

#include "Arduino.h"
#include <Servo.h>
#include "SharpIR.h"
#include "Motor.h"
#include "Pins.cpp"
#include "variables.cpp"

class BracoCopo{
public:
    BracoCopo();
    BracoCopo(int servoPulso, int servoGarra, int sharpGarra, Motor m);
    BracoCopo(int servoPulso, int servoGarra, int sharpGarra, int mSharp_D, int mSharp_E, Motor m);

    int getSharp();

    inline void setSharpGarra(int sharpGarra){this->sharpGarra = sharpGarra;}
    inline void attachPulso(int porta){pulso.attach(porta);}
    inline void attachGarra(int porta){garra.attach(porta);}
    void attachMotor(Motor m);
    inline void set_mSharp_E(int porta){mSharp_E = porta;}
    inline void set_mSharp_D(int porta){mSharp_D = porta;}

    inline int getSharpGarra(){return sharpGarra;}
    inline int get_mSharp_D(){return mSharp_D;}
    inline int get_mSharp_E(){return mSharp_E;}
    inline Motor getMotor(){return motorBraco;}
    inline Servo getPulso(){return pulso;}
    inline Servo getGarra(){return garra;}

    inline bool isFDC(int porta){return digitalRead(porta);}

    void iniciar();
    void tryGetTerrine();
    void recolherBraco();

    void dropLeite();
    
private:
    Servo pulso;
    Servo garra;
    int sharpGarra;
    int mSharp_E; // analogico esquerda
    int mSharp_D; // analógico direita
    Motor motorBraco;
};

#endif

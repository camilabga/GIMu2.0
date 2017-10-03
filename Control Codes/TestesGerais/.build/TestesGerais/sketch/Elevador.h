#ifndef ELEVADOR_H
#define ELEVADOR_H

#include "Arduino.h"
#include "Pins.cpp"
#include "variables.cpp"
#include "Motor.h"

class Elevador {
private:
    Motor mElevador;
    int stage; // 1 -> 2 -> 3
public:
    Elevador(Motor m, int stage);
    Elevador();

    void attachMotor(Motor m);
    inline Motor getMotor(){return mElevador;}
    inline void setStage(int s){stage = s;}
    inline int getStage(){return stage;}

    void goToStage01();
    void goToStage02();
    void goToStage03();

    inline int whatStage(){return analogRead(FDC_ELEVADOR);}
};

#endif
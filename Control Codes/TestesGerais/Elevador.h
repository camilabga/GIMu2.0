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

    // vai direto para os "andares" -> só para de subir
    // quando chega no fim de curso determinado
    void goToStage01(); 
    void goToStage02();
    void goToStage03();

    // manda subir, nao esta em loop, mas pára se chegar
    // no fim de curso
    void upToStage02();
    void upToStage03();

    // manda descer, nao esta em loop, mas pára se chegar
    // no fim de curso
    void downToStage01();
    void downToStage02();

    inline void stop(){mElevador(0,0);}

    inline int whatStage(){return analogRead(FDC_ELEVADOR);}
};

#endif
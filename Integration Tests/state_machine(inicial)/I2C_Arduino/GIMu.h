#ifndef GIMu_h
#define GIMu_h

#include "Arduino.h"
#include "Motor.h"
#include "SharpIR.h"
#include "Pins.cpp"
#include "variables.cpp"
#include "BracoCopo.h"

class GIMu{
  public:
    GIMu(Motor d, Motor e);
    GIMu(BracoCopo b);
    GIMu(Motor d, Motor e, BracoCopo b);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);
    void taxearDireita();

    void getSharps();
    inline int getDistancia(int i){return sharpsBase[i];}
    int getSharp(int porta);
    
    void follow_wall_to_cup();

    void getTerrine();
    int sharpsBase[6];
  private:
    Motor Mright;
    Motor Mleft;
    
    BracoCopo bracoCopo;
};

#endif

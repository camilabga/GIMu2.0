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

    void follow_wall_to_cup();

    int getSharp(int porta);

  private:
    Motor Mright;
    Motor Mleft;
    int sharpsBase[6];
    BracoCopo bracoCopo;
};

#endif

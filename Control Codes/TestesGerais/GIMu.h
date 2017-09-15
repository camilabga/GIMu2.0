#ifndef GIMu_h
#define GIMu_h

#include "Motor.h"
#include "SharpIR.h"
#include "Pins.cpp"

class GIMu{
  public:
    GIMu(Motor d, Motor e);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);
    void getSharps();
    inline int getDistancia(int i){return sharpsBase[i]}

    bool follow_wall();

  private:
    Motor Mright;
    Motor Mleft;
    int sharpsBase[6];

    int getSharp(int porta);
};

#endif

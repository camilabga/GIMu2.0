#ifndef GIMu_h
#define GIMu_h

#include "Motor.h"
#include "SharpIR.h"

class GIMu{
  public:
    GIMu(Motor d, Motor e);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);
    int getDist(int porta);

  private:
    Motor Mright;
    Motor Mleft;
};

#endif

#ifndef GIMu_h
#define GIMu_h

#include "Arduino.h"
#include "Motor.h"

class GIMu{
  public:
    GIMu(Motor d, Motor e);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);
    
  private:
    Motor Mright;
    Motor Mleft;
    
};

#endif

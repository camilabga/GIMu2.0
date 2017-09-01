#ifndef GIMu_h
#define GIMu_h

#include "Motor.h"
#include "SharpSensor.h"

class GIMu{
  public:
    GIMu(Motor d, Motor e);
    GIMu(Motor d, Motor e, int n, SharpSensor &S);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);

  private:
    Motor Mright;
    Motor Mleft;
    SharpSensor *sharpSensors;  
};

#endif
#ifndef GIMu_h
#define GIMu_h

#include "Motor.h"
#include "SharpSensor.h"
#include "ListaSharp.h"

class GIMu{
  public:
    GIMu(Motor d, Motor e);
    GIMu(Motor d, Motor e, ListaSharp sensoresB);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);

  private:
    Motor Mright;
    Motor Mleft;
    ListaSharp sensoresBase;
};

#endif
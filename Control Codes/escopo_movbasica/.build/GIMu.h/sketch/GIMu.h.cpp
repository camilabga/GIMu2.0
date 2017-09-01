#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.h"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.h"
#ifndef GIMu_h
#define GIMu_h

#include "Motor.h"
#include "SharpSensor.h"
#include "ListaSharp.h"

#line 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
void setup();
#line 12 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
void loop();
#line 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
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
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
#include "GIMu.h"

Motor direito(6, 9);
Motor esquerdo(3, 5);

GIMu robo (direito, esquerdo);

void setup() {
  
}

void loop() {
  robo.moveFrente(255);
  delay(1000);
  robo.moveTras(255);
  delay(1000);
  robo.moveTank(120, 240);
  delay(1000); 
}

#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/BracoCopo.h"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/BracoCopo.h"
#ifndef Garra_h
#define Garra_h

#include "Arduino.h"
#include <Servo.h>
#include "SharpIR.h"
#include "Motor.h"
#include "variables.cpp"

class BracoCopo{
public:
    BracoCopo();
    BracoCopo(int servoPulso, int servoGarra, int sharpGarra, Motor m);

    int getSharp();

    inline int getSharpGarra(){return sharpGarra;}

    inline void setSharpGarra(int sharpGarra){this->sharpGarra = sharpGarra;}
    inline void attachPulso(int porta){pulso.attach(porta);}
    inline void attachGarra(int porta){garra.attach(porta);}
    void attachMotor(Motor m);
    
private:
    Servo pulso;
    Servo garra;
    int sharpGarra;
    Motor motorBraco;
};

#endif
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#include "GIMu.h"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);

GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(115200);
  robo.follow_wall_to_cup();

}

void loop() {
  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(200, -200);
   delay(2000);
  /* ###*/

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));*/

  /*Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
  Serial.print(" S3: ");
  Serial.println(robo.getSharp(SH_FRENTE_ESQUERDA));*/
  /*erial.print(" S4: ");
  Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));*/
  
  // ###

  
  /* ### Teste mov + sharp ### */

  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));

  robo.moveTank(LOOKING_SPEED, -LOOKING_SPEED); */ 
  

}


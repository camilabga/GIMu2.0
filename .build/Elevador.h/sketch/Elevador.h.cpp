#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/Elevador.h"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/Elevador.h"
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

    inline void stop(){mElevador.moveMotor(0,0);}

    inline int whatStage(){return analogRead(FDC_ELEVADOR);}
};

#endif
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#include "GIMu.h"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);

Motor mbraco(MBRACO1, MBRACO2);
BracoCopo braco(SERVOG_PULSO, SERVOG_DEDO, SH_GARRA, MSH_GARRA_D, MSH_GARRA_E, mbraco);

Motor mElevator(DC_ELEVADOR1, DC_ELEVADOR0);
Elevador elevador(mElevator, 3);

GIMu robo (direito, esquerdo, braco, elevador);

void setup() {
  Serial.begin(9600);
}

void loop() {
  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(200, -200);
   delay(2000);*/
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
  /*Serial.print(" S4: ");
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
  
  // ### teste seguir parede ###
 
  //robo.follow_wall_to_cup();

  // ### teste pegar copo ###
  //robo.getTerrine();

  // ### TESTE ELEVADOR ###
  elevador.goToStage03();
  elevador.goToStage01();
  elevador.goToStage02();

  // ### TESTE GARRA ###
  /*braco.tryGetTerrine();
  braco.recolherBraco();*/
  
}


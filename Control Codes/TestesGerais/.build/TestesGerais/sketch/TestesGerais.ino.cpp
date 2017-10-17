#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#include "GIMu.h"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);

Motor mbraco(MBRACO1, MBRACO2);
BracoCopo braco(SERVOG_PULSO, SERVOG_DEDO, SH_GARRA, MSH_GARRA_D, MSH_GARRA_E, mbraco);

Motor mElevator(DC_ELEVADOR1, DC_ELEVADOR0);
Elevador elevador(mElevator, 1);

GIMu robo (direito, esquerdo, braco, elevador);

//GIMu robo (direito, esquerdo);
//GIMu robo (elevador);
//GIMu robo(braco);

//Servo teste;

char in;

#line 22 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup();
#line 27 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void loop();
#line 22 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup() {
  Serial.begin(9600);
  //teste.attach(2);
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
  Serial.print(robo.getSharp(SH_DIREITA_FRENTE));
  Serial.print("  || S2: ");
  Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH_FRENTE_ESQUERDA));
  Serial.print("  || S4: ");
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
  /*elevador.goToStage03();
  elevador.goToStage01();
  elevador.goToStage02();*/

  // ### TESTE GARRA ###
  /*braco.tryGetTerrine();
  delay(500);
  braco.recolherBraco();
  delay(2000);*/
  //teste.write(130);
  
  /*
  // TESTE ESTACIONAR (LER SENSORES)
  in = ' ';

  if (Serial.available() > 0) {
    in = Serial.read();

    switch(in){
      case 'w':
        robo.moveFrente(PARKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 's':
        robo.moveTras(PARKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'a':
        robo.moveTank(-TURNING_SPEED, TURNING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'd':
        robo.moveTank(TURNING_SPEED, -TURNING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'q':
        robo.moveTank(LOOKING_SPEED, PARKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'e':
        robo.moveTank(PARKING_SPEED, LOOKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'f':
        Serial.print(" S2: ");
        Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
        Serial.print(" S3: ");
        Serial.print(robo.getSharp(SH_FRENTE_ESQUERDA));
        Serial.print(" S4: ");
        Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
        Serial.print(" S5: ");
        Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));
      break;
    }

  }*/


}


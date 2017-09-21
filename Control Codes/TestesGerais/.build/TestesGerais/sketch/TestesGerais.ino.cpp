#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#include "GIMu.h"
#include "Pins.cpp"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

#line 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup();
#line 12 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void loop();
#line 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup() {
  Serial.begin(9600);
}

void loop() {
  /* ### Teste de Movimentação:*/
   robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(120, 240);
   delay(2000);
  /* ###*/

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH1));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH0));*/

  /*Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH2));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH3));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH4));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH5));
  delay(500);*/
  // ###

  
  //robo.follow_wall_to_cup();  

}


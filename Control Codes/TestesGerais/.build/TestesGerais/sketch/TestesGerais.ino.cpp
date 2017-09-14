#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#include "GIMu.h"
#include "Pins.cpp"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

// Teste com Bluetooth
SoftwareSerial bt(10, 11); // RX, TX

#line 11 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup();
#line 17 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void loop();
#line 11 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup() {
  //Teste com Bluetooth
  bt.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  // ### Teste de Movimentação:
  // robo.moveFrente(255);
  // delay(1000);
  // robo.moveTras(255);
  // delay(1000);
  // robo.moveTank(120, 240);
  // delay(1000);
  // ###

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getDist(SH0));
  Serial.print(" S1: ");
  Serial.print(robo.getDist(SH1));
  Serial.print(" S2: ");
  Serial.print(robo.getDist(SH2));
  Serial.print(" S3: ");
  Serial.print(robo.getDist(SH3));
  Serial.print(" S4: ");
  Serial.print(robo.getDist(SH4));
  Serial.print(" S5: ");
  Serial.println(robo.getDist(SH5));*/

  // Teste com bluetooth
  if (bt.available())
    S=bt.read();
  if(S=='w')
    robo.moveFrente(255);
  if(S=='s') 
    robo.moveTras(255);
  if(S=='d') //direita
    robo.moveTank(255, 120);
  if(S=='a') //esquerda
    robo.moveTank(120, 255);
  if(S=='p') //esquerda
    robo.moveTank(0, 0);
  if(S=='q')
      digitalWrite(13,!digitalRead(13));

}


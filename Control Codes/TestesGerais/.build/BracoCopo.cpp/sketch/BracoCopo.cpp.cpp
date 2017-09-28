#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/BracoCopo.cpp"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/BracoCopo.cpp"
#include "BracoCopo.h"

BracoCopo::BracoCopo(){}

BracoCopo::BracoCopo(int servoPulso, int servoGarra, int sharpGarra, Stepper_M motorPasso){
  pulso.attach(servoPulso);
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup();
#line 11 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void loop();
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
  garra.attach(servoGarra);
  this->sharpGarra = sharpGarra;
  
}

int BracoCopo::getSharp(int porta){
    SharpIR SharpIR(porta, 1080);
    byte n = 20;
    int media, desvio;
    int x[20];
    long unsigned soma=0;
    for(int i=0;i<n;i++){
        x[i] = SharpIR.distance();
        soma += x[i];  // this returns the distance to the object you're measuring
    }
    media = (soma/n);
    soma = 0;
    for (int i =0;i< n; i++) {
      soma += (x[i]-media)*(x[i]-media);
    }

    desvio = soma/n;

    if (desvio > VALID_SHARP || media > 80) {
      return -1; // é ruido
    } else {
      return media; // n é ruido
    }
}
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#include "GIMu.h"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(115200);
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
  /*Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));*/
  
  // ###

  robo.follow_wall_to_cup();
  

}


#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
#include "Arduino.h"
#include "GIMu.h"

GIMu::GIMu(Motor d, Motor e){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup();
#line 13 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void loop();
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());
}

GIMu::GIMu(BracoCopo b){
    bracoCopo.setPulso(b.getPulso());
    bracoCopo.setGarra(b.getGarra());
    bracoCopo.setSharpGarra(b.getSharpGarra());
    bracoCopo.setMotorPasso(b.getMotorPasso());
}

GIMu::GIMu(Motor d, Motor e, BracoCopo b){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());

    bracoCopo.setPulso(b.getPulso());
    bracoCopo.setGarra(b.getGarra());
    bracoCopo.setSharpGarra(b.getSharpGarra());
    bracoCopo.setMotorPasso(b.getMotorPasso());
}

void GIMu::moveFrente(int velocidade){
    Mright.moveMotor(velocidade, 1);
    Mleft.moveMotor(velocidade, 1);
}

void GIMu::moveTras(int velocidade){
    Mright.moveMotor(velocidade, 0);
    Mleft.moveMotor(velocidade, 0);
}

void GIMu::moveTank(int pwm_esquerdo, int pwm_direito){
    if (pwm_esquerdo < 0) {
        Mleft.moveMotor(-pwm_esquerdo, 0);
    } else {
        Mleft.moveMotor(pwm_esquerdo, 1);
    }

    if (pwm_direito < 0) {
        Mright.moveMotor(-pwm_direito, 0);
    } else {
        Mright.moveMotor(pwm_direito, 1);
    }
}

int GIMu::getSharp(int porta){
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
     
    /*const int media = 50;
    int valueSensorAux = 0;
    int total = 0;
    int cont = 0;
    
    while (cont < media){
        total += analogRead(porta);
        cont++;
    }
    return (total / media);*/
}

void GIMu::getSharps(){
    sharpsBase[0] = getSharp(SH_DIREITA_TRAS);
    sharpsBase[1] = getSharp(SH_DIREITA_FRENTE);
    sharpsBase[2] = getSharp(SH_FRENTE_DIREITA);
    sharpsBase[3] = getSharp(SH_FRENTE_ESQUERDA);
    sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
    sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
}

void GIMu::taxear(int i1, int i2){

}

void GIMu::follow_wall_to_cup() {
    bool found_wall = false;
    bool found_terrine_area = false;
    while (!found_terrine_area){
                   //long int init = micros();
        //getSharps(); // pega os valores dos sharps
                  //Serial.println((float)(micros() - init)/1000000);
        if (!found_wall){
            sharpsBase[2] = getSharp(SH_FRENTE_DIREITA);
            sharpsBase[3] = getSharp(SH_FRENTE_ESQUERDA);
            if ((sharpsBase[2] == -1 || sharpsBase[3] == -1) || (sharpsBase[2] >= DIST_TURN01 || sharpsBase[3] >= DIST_TURN01)) {
                moveFrente(LOOKING_SPEED);
                Serial.println("Segue em frente");
            } else if (sharpsBase[2] < DIST_TURN01 || sharpsBase[3] < DIST_TURN01) {
                Serial.println("Achou Parede");

                do {
                   //getSharps();
                   sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
                   sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
                   moveTank(TURNING_SPEED, -TURNING_SPEED);
                } while(!(sharpsBase[4] != -1 || sharpsBase[5] != -1) || (abs(sharpsBase[4]-sharpsBase[5]) > SHARP_DIFF));
                
                found_wall = true;
                moveFrente(0);
                delay(1000);
            }

        } else {
            sharpsBase[2] = getSharp(SH_FRENTE_DIREITA);
            sharpsBase[3] = getSharp(SH_FRENTE_ESQUERDA);
            Serial.print(" S2: ");
            Serial.print(sharpsBase[2]);
            Serial.print(" S3: ");
            Serial.println(sharpsBase[3]);
            if ((sharpsBase[2] <= DIST_TURN01 && sharpsBase[2] != -1) || (sharpsBase[3] <= DIST_TURN01 && sharpsBase[3] != -1)) {
                found_terrine_area = true;
                moveFrente(0);
                Serial.println("Achei o caralho todo");
            } else {
                moveFrente(LOOKING_SPEED);
            }
        }
    }
}

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
  

}


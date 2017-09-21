#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
#include "Arduino.h"
#include "GIMu.h"

GIMu::GIMu(Motor d, Motor e){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
#line 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup();
#line 12 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void loop();
#line 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
    Mleft.setPinTras(e.getPinTras());
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

    if (desvio > VALID_SHARP) {
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
    sharpsBase[0] = getSharp(SH0);
    sharpsBase[1] = getSharp(SH1);
    sharpsBase[2] = getSharp(SH2);
    sharpsBase[3] = getSharp(SH3);
    sharpsBase[4] = getSharp(SH4);
    sharpsBase[5] = getSharp(SH5);
}

void GIMu::follow_wall_to_cup() {
    bool found_wall = false;
    bool found_terrine_area = false;
    while (!found_terrine_area){
        getSharps(); // pega os valores dos sharps
        if (!found_wall){
            if ((sharpsBase[0] == -1 || sharpsBase[1] == -1) || (sharpsBase[0] >= DIST_TURN01 || sharpsBase[1] >= DIST_TURN01)) {
                moveFrente(LOOKING_SPEED);
                Serial.println("Segue em frente");
            } else if (sharpsBase[0] < DIST_TURN01 || sharpsBase[1] < DIST_TURN01) {
                Serial.println("Achou Parede");
                moveFrente(0);
                do {
                    getSharps();
                    moveTank(TURNING_SPEED, -TURNING_SPEED);
                    Serial.print(" S2: ");
                    Serial.print(sharpsBase[2]);
                    Serial.print(" S3: ");
                    Serial.println(sharpsBase[3]);
                } while(!(sharpsBase[2] != -1 || sharpsBase[3] != -1) || (abs(sharpsBase[2]-sharpsBase[3]) > SHARP_DIFF));
                moveFrente(0);
                found_wall = true;
            }

        } else {
            if ((sharpsBase[0] != -1 || sharpsBase[1] != -1) && (sharpsBase[0] <= DIST_TURN01 || sharpsBase[1] <= DIST_TURN01)) {
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
#include "Pins.cpp"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(9600);
}

void loop() {
  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(120, 240);
   delay(2000);*/
  /* ###*/

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH0));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH1));*/

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

  
  robo.follow_wall_to_cup(); 

}


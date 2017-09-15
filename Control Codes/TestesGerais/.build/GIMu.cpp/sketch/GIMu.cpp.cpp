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
    Mright.moveMotor(pwm_direito, 1);
    Mleft.moveMotor(pwm_esquerdo, 1);
}

int GIMu::getSharp(int porta){
    SharpIR SharpIR(porta, 1080);
    byte media = 10;
    long unsigned soma=0;
    for(int i=0;i<media;i++)
        soma += SharpIR.distance();  // this returns the distance to the object you're measuring
    return (soma/media);
     
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
        getSharps();
        if (!found_wall){
            if (sharpsBase[0] > DIST_TURN0 && sharpsBase[1] > DIST_TURN0) {
                if (abs(sharpsBase[0] - sharpsBase[1]) > SHARP_DIFF){
                    if (sharpsBase[0] > sharpsBase[1]){
                        moveTank(ADJUSTING_SPEED2, ADJUSTING_SPEED1);
                    } else {
                        moveTank(ADJUSTING_SPEED1, ADJUSTING_SPEED2);
                    }
                }
            } else {
                moveTras(LOOKING_SPEED);
                delay(TEMPO_DE_RE);
                do {
                    moveTank(TURNING_SPEED, 0);
                    getSharps();
                } while(abs(sharpsBase[2] - sharpsBase[3]) > SHARP_DIFF);
            }           

        } else {
            if (abs(sharpsBase[2] - sharpsBase[3]) <= SHARP_DIFF) {
                moveFrente(LOOKING_SPEED);
                if (sharpsBase[2] > sharpsBase[3]){
                    moveTank(ADJUSTING_SPEED2, ADJUSTING_SPEED1);
                } else {
                    moveTank(ADJUSTING_SPEED1, ADJUSTING_SPEED2);
                }
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
  //Serial.begin(9600);
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
  Serial.print(robo.getSharp(SH0));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(SH1));
  Serial.print(" S2: ");
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


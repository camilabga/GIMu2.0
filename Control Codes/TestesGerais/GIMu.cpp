#include "Arduino.h"
#include "GIMu.h"

GIMu::GIMu(Motor d, Motor e){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
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
            if (sharpsBase[0] > DIST_TURN0 && sharpsBase[1] > DIST_TURN0) {
                if (abs(sharpsBase[2] - sharpsBase[3]) <= SHARP_DIFF) {
                    moveFrente(LOOKING_SPEED);
                    if (sharpsBase[2] > sharpsBase[3]){
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
                found_terrine_area = true;
            }
        }
    }
}
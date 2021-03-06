#include "Elevador.h"

Elevador::Elevador(Motor m){
    mElevador.setPinFrente(m.getPinFrente());
    mElevador.setPinTras(m.getPinTras());
    stage=0;
}

Elevador::Elevador(){
    stage = 1;
}

void Elevador::attachMotor(Motor m){
    mElevador.setPinFrente(m.getPinFrente());
    mElevador.setPinTras(m.getPinTras());
}

void Elevador::goToStage01(){
    while (whatStage() < POSICAO01) {
        mElevador.moveMotor(255, 1);
        Serial.println(whatStage());
    }
    delay(1500);
    mElevador.moveMotor(0, 0);
    delay(1000);
    stage = 1;
}

void Elevador::goToStage02(){
    while (whatStage() > POSICAO02) {
        mElevador.moveMotor(255, 0);
    }
    mElevador.moveMotor(0, 0);
    while (whatStage() < POSICAO02) {
        mElevador.moveMotor(255, 1);
    }
    mElevador.moveMotor(0, 0);
    delay(1000);
    stage = 2;
}

void Elevador::goToStage03(){
    while (whatStage() > POSICAO03) {
        mElevador.moveMotor(255, 0);
        Serial.println(whatStage());
    }
    mElevador.moveMotor(0, 0);
    delay(1000);
    
    stage = 3;
}

void Elevador::upToStage02(){
    if (whatStage() > POSICAO02) {
        mElevador.moveMotor(255,0);
    } else {
        mElevador.moveMotor(0, 0);
        stage = 2;
    }
}

void Elevador::upToStage03(){
    if (whatStage() > POSICAO03) {
        mElevador.moveMotor(255,0);
        Serial.println(whatStage());
    } else {
        mElevador.moveMotor(0,0);
        stage = 3;
    }
}

void Elevador::downToStage01(){
    if (whatStage() < POSICAO01) {
        mElevador.moveMotor(255,1);
    } else {
        mElevador.moveMotor(0, 0);
        stage = 1;
    }
}

void Elevador::downToStage02(){
    if (whatStage() < POSICAO02) {
        mElevador.moveMotor(255,1);
    } else {
        mElevador.moveMotor(0, 0);
        stage = 2;
    }
}
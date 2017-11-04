#include "Elevador.h"

Elevador::Elevador(Motor m, int stage){
    mElevador.setPinFrente(m.getPinFrente());
    mElevador.setPinTras(m.getPinTras());

    this->stage = stage;
}

Elevador::Elevador(){
    stage = 1;
}

void Elevador::attachMotor(Motor m){
    mElevador.setPinFrente(m.getPinFrente());
    mElevador.setPinTras(m.getPinTras());
}

void Elevador::goToStage01(){
    while (whatStage() > POSICAO01) {
        mElevador.moveMotor(255, 1);
    }
    mElevador.moveMotor(0, 0);
    delay(1000);
    stage = 1;
}

void Elevador::goToStage02(){
    if (stage == 1) {
        while (whatStage() < POSICAO02) {
            mElevador.moveMotor(255, 0);
        }
        mElevador.moveMotor(0, 0);
    } else if (stage == 3) {
        while (whatStage() > POSICAO02) {
            mElevador.moveMotor(255, 1);
        }
        mElevador.moveMotor(0, 0);
        delay(1000);
    }

    stage = 2;
}

void Elevador::goToStage03(){
    while (whatStage() < POSICAO03) {
        mElevador.moveMotor(255, 0);
    }
    mElevador.moveMotor(0, 0);
    delay(1000);
    
    stage = 3;
}

void Elevador::upToStage02(){
    if (whatStage() < POSICAO02) {
        mElevador.moveMotor(255,0);
    } else {
        mElevador.moveMotor(0, 0);
        stage = 2;
    }
}

void Elevador::upToStage03(){
    if (whatStage() < POSICAO03) {
        mElevador.moveMotor(255,0);
    } else {
        mElevador.moveMotor(0,0);
        stage = 3;
    }
}

void Elevador::downToStage01(){
    if (whatStage() > POSICAO01) {
        mElevador.moveMotor(255,1);
    } else {
        mElevador.moveMotor(0, 0);
        stage = 1;
    }
}

void Elevador::downToStage02(){
    if (whatStage() > POSICAO02) {
        mElevador.moveMotor(255,1);
    } else {
        mElevador.moveMotor(0, 0);
        stage = 2;
    }
}
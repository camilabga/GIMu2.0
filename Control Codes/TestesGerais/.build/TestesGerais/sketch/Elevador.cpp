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
    while (whatStage() > POSICAO01 || whatStage() < CONSIDER_NULL) {
        mElevador.moveMotor(255, 0);
    }
    mElevador.moveMotor(0, 0);
    stage = 1;
}

void Elevador::goToStage02(){
    if (stage == 1) {
        while (whatStage() < POSICAO02 || whatStage() < CONSIDER_NULL) {
            mElevador.moveMotor(255, 1);
        }
        mElevador.moveMotor(0, 0);
    } else if (stage == 3) {
        while (whatStage() > POSICAO02 || whatStage() < CONSIDER_NULL) {
            mElevador.moveMotor(255, 0);
        }
        mElevador.moveMotor(0, 0);
    }

    stage = 2;
}

void Elevador::goToStage03(){
    while (whatStage() < POSICAO03 || whatStage() < CONSIDER_NULL) {
        mElevador.moveMotor(255, 1);
    }

    stage = 3;
}
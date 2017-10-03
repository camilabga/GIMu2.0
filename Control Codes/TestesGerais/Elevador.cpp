#include "Elevador.h"

Elevador::Elevador(Motor m, int stage){
    mElevador.setPinFrente(m.getPinFrente());
    mElevador.setPinTras(m.getPinTras());

    this->stage = 1;

    if (stage == 1) {
        goToStage01();
    } else if (stage == 2) {
        goToStage02();
    } else {
        goToStage03();
    }

}

Elevador::Elevador(){
    stage = 0;
}

void Elevador::attachMotor(Motor m){
    mElevador.setPinFrente(m.getPinFrente());
    mElevador.setPinTras(m.getPinTras());
}

void Elevador::goToStage01(){
    while (whatStage() > POSICAO01 || whatStage() < CONSIDER_NULL) {
        mElevador.moveMotor(255, 0);
    }
    stage = 1;
}

void Elevador::goToStage02(){
    if (stage == 1) {
        while (whatStage() < POSICAO02 || whatStage() < CONSIDER_NULL) {
            mElevador.moveMotor(255, 1);
        }
    } else if (stage == 3) {
        while (whatStage() > POSICAO02 || whatStage() < CONSIDER_NULL) {
            mElevador.moveMotor(255, 0);
        }
    }

    stage = 2;
}

void Elevador::goToStage03(){
    while (whatStage() < POSICAO03 || whatStage() < CONSIDER_NULL) {
        mElevador.moveMotor(255, 1);
    }

    stage = 3;
}
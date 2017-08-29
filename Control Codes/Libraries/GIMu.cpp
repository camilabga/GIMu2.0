#include "Arduino.h"
#include "Motor.h"
#include "GIMu.h"

GIMu::GIMu(Motor d, Motor e){
    Mright.pinFrente = d.pinFrente;
    Mright.pinTras = d.pinTras;
    Mleft.pinFrente = e.pinFrente;
    Mleft.pinTras = e.pinTras;
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

/*int GIMu::getSharp(int pin){
    const byte media = 50;
    int valueSensorAux = 0;
    int total = 0;
    int cont = 0;

    valueSensorAux = analogRead(sensorPin[pin]);
    while (cont < media)
    {
        total = valueSensorAux + total;
        cont++;
    }

    return total / media;
}*/
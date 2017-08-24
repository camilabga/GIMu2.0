#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor{
public:
    Motor();
    Motor(int f, int t);
    void moveMotor(int velocidade, bool direcao);

    int pinFrente;
    int pinTras;
};

#endif

#ifndef Stepper_M_h
#define Stepper_M_h

#include "Arduino.h"
#include "variables.cpp"
#include "Pins.cpp"

class Stepper_M {
private:
    int bobina[4];
    int StepsRev;
public:
    Stepper_M();
    Stepper_M(int SPR, int b1, int b2, int b3, int b4);
    void gira(float N_voltas, unsigned velocidade);
};

#endif
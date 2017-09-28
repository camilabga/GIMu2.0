#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor{
public:
    Motor();
    Motor(int f, int t);
    void moveMotor(int velocidade, bool direcao);

    inline int getPinFrente(){return pinFrente;}
    inline int getPinTras(){return pinTras;}

    inline void setPinFrente(int pino){pinFrente = pino;}
    inline void setPinTras(int pino){pinTras = pino;}

private:
    int pinFrente;
    int pinTras;
};

#endif

#ifndef ListaSharp_h
#define ListaSharp_h

#include "Arduino.h"
#include "SharpSensor.h"

class ListaSharp{
private:
    SharpSensor *sharps;
    unsigned n;

public:
    ListaSharp();
    ListaSharp(unsigned num);
    ListaSharp(unsigned tam, int portas[]);

    inline unsigned size(){return n;}
};

#endif
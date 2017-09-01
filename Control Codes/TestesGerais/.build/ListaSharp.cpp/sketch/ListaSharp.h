#ifndef ListaSharp_h
#define ListaSharp_h

#include "Arduino.h"

class ListaSharp{
private:
    SharpSensor *sharps;
    unsigned n;

public:
    ListaSharp();
    ListaSharp(unsigned num);
    ListaSharp(unsigned tam, int portas[]);
};

#endif
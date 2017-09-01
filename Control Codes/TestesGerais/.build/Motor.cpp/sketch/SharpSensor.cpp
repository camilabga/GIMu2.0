#include "SharpSensor.h"

SharpSensor::SharpSensor(int porta){
    this->porta = porta;
    distancia = 0;
}

SharpSensor::SharpSensor(){
    porta = 13;
    distancia = 0;
}

void SharpSensor::getSharp(){
    const byte media = 50;
    int valueSensorAux = 0;
    int total = 0;
    int cont = 0;

    valueSensorAux = analogRead(porta);
    while (cont < media){
        total = valueSensorAux + total;
        cont++;
    }

    distancia = total / media;
}

void SharpSensor::operator=(const SharpSensor &SS){
    if (this != SS) {
        porta = 13;
        distancia = 0;
    }

    porta = SS.porta;
    distancia = SS.distancia;
}
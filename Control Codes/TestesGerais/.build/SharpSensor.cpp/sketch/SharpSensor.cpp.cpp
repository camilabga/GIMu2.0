#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/SharpSensor.cpp"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/SharpSensor.cpp"
#include "SharpSensor.h"

SharpSensor::SharpSensor(int porta){
    this->porta = porta;
    distancia = 0;
}
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
void setup();
#line 11 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
void loop();
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"

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
    porta = 13;
    distancia = 0;

    porta = SS.porta;
    distancia = SS.distancia;
}
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
#include "GIMu.h"

Motor direito(6, 9);
Motor esquerdo(3, 5);
GIMu robo (direito, esquerdo);

void setup() {
  
}

void loop() {
  robo.moveFrente(255);
  delay(1000);
  robo.moveTras(255);
  delay(1000);
  robo.moveTank(120, 240);
  delay(1000); 
}


#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/Motor.cpp"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/Motor.cpp"
#include "Motor.h"

Motor::Motor(){
  pinFrente = 13;
  pinTras = 13;
}
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
void setup();
#line 11 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
void loop();
#line 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"

Motor::Motor(int f, int t){
  pinFrente = f;
  pinTras = t;
  pinMode(pinFrente, OUTPUT);
  pinMode(pinTras, OUTPUT);
}

void Motor::moveMotor(int velocidade, bool direcao){
  // direcao: 0 -> ré
        //    1 -> frente
  if (direcao) {
    analogWrite(pinFrente, velocidade);
    analogWrite(pinTras, 0);
  } else {
    analogWrite(pinFrente, 0);
    analogWrite(pinTras, velocidade);
  }
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


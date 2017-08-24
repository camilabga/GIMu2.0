#include "Arduino.h"
#include "Motor.h"

Motor::Motor(){
  pinFrente = 13;
  pinTras = 13;
}

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

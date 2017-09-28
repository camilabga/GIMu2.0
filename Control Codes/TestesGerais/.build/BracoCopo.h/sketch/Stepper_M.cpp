#include "Stepper_M.h"

Stepper_M::Stepper_M(){
  
}

Stepper_M::Stepper_M(int SPR, int b1, int b2, int b3, int b4) {
    bobina[0] = b1;
    bobina[1] = b2;
    bobina[2] = b3;
    bobina[3] = b4;
    StepsRev = SPR;
    for (int i = 0; i < 4; i++)
        pinMode(bobina[i], OUTPUT);
}
  
  
void Stepper_M::gira(float N_voltas, unsigned velocidade) { //girar motor com meio passo
    for (int rev = 0; rev < abs((int)(N_voltas * StepsRev)) ; rev++) {
        if (N_voltas > 0.0)//horario
        for (int i = 0; i <= 3; i++) {
            digitalWrite(bobina[(i + 1) % 4], 1);
            delay(velocidade);
            digitalWrite(bobina[i], 0);
            delay(velocidade);
        }
        else  //anti-horario
        for (int i = 3; i >= 0; i--) {
            digitalWrite(bobina[i], 1);
            delay(velocidade);
            digitalWrite(bobina[(i + 1) % 4], 0);
            delay(velocidade);
        }
    }
    digitalWrite(bobina[0], 0);
}
  /*
  
  char S;
  void loop() {
    Stepper_M motor(500, 2, 3, 4, 5);
    if(Serial.available())
      S = Serial.read();
    if (S == '1') {
      digitalWrite(13, 1);
      motor.gira(1, 3);
      digitalWrite(13, 0);
    }
    if (S == '2') {
      digitalWrite(13, 1);
      motor.gira(-1, 2);
      digitalWrite(13, 0);
    }
    S = ' ';
  }*/
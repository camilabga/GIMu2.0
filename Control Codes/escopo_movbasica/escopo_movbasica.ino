#include <Motor.h>
#include <GIMu.h>

Motor direito(6, 9);
Motor esquerdo(3, 5);
GIMu robo (direito, esquerdo);

void setup() {
  
}

void loop() {
  robo.moveFrente(255);
  robo.moveTras(255);
  robo.moveTank(120, 240);
}

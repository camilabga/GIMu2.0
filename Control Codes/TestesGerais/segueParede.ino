#include "GIMu.h"
#include "Pins.cpp"
#include "variables.cpp"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

void setup() {
    
}

void loop() {
    robo.follow_wall();
}
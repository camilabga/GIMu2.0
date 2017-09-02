#include "GIMu.h"
#include "Pins.cpp"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // ### Teste de Movimentação:
  // robo.moveFrente(255);
  // delay(1000);
  // robo.moveTras(255);
  // delay(1000);
  // robo.moveTank(120, 240);
  // delay(1000);
  // ###

  // ### Teste dos sensores Sharps:
  Serial.print(" S0: ");
  Serial.print(robo.getDist(SH0));
  Serial.print(" S1: ");
  Serial.print(robo.getDist(SH1));
  Serial.print(" S2: ");
  Serial.print(robo.getDist(SH2));
  Serial.print(" S3: ");
  Serial.print(robo.getDist(SH3));
  Serial.print(" S4: ");
  Serial.print(robo.getDist(SH4));
  Serial.print(" S5: ");
  Serial.println(robo.getDist(SH5));
  delay(500);
  // ###

}
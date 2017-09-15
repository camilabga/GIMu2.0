# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/segueParede.ino"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/segueParede.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/segueParede.ino" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/segueParede.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
GIMu robo (direito, esquerdo);

void setup() {

}

void loop() {
    robo.follow_wall();
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
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
  Serial.print(robo.getSharp(0 /* frente direita */));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(1 /* frente esquerda*/));
  Serial.print(" S2: ");
  Serial.print(robo.getSharp(2 /* esquerda frente*/));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(3 /* esquerda tras*/));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(4 /* direita frente*/));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(5 /* direita tras*/));
  delay(500);
  // ###

}

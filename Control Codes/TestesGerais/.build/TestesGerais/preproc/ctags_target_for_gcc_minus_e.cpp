# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
GIMu robo (direito, esquerdo);

// Teste com Bluetooth
SoftwareSerial bt(10, 11); // RX, TX

void setup() {
  //Teste com Bluetooth
  bt.begin(9600);
  pinMode(13,0x1);
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
  /*Serial.print(" S0: ");
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
  Serial.println(robo.getDist(SH5));*/

  // Teste com bluetooth
  if (bt.available())
    S=bt.read();
  if(S=='w')
    robo.moveFrente(255);
  if(S=='s')
    robo.moveTras(255);
  if(S=='d') //direita
    robo.moveTank(255, 120);
  if(S=='a') //esquerda
    robo.moveTank(120, 255);
  if(S=='p') //esquerda
    robo.moveTank(0, 0);
  if(S=='q')
      digitalWrite(13,!digitalRead(13));

}

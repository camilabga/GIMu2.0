# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(9600);
}

void loop() {
  /* ### Teste de Movimentação:*/
   robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(120, 240);
   delay(2000);
  /* ###*/

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH1));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH0));*/

  /*Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH2));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH3));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH4));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH5));
  delay(500);*/
  // ###


  //robo.follow_wall_to_cup();  

}

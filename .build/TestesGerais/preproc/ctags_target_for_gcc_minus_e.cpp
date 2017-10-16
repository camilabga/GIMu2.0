# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(11 /*Esquerdo*/, 10);
Motor direito(8 /*Direito*/, 9);

Motor mbraco(4, 5);
BracoCopo braco(3, 2, 8, 9, 10, mbraco);

/*Motor mElevator(DC_ELEVADOR1, DC_ELEVADOR0);
Elevador elevador(mElevator, 3);

GIMu robo (direito, esquerdo, braco, elevador);*/

GIMu robo (braco);

Servo teste;

char in;

void setup() {
  Serial.begin(9600);
  //teste.attach(2);
}

void loop() {
  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(200, -200);
   delay(2000);*/
  /* ###*/

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(SH_DIREITA_FRENTE));
  Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH_FRENTE_ESQUERDA));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));*/

  // ###


  /* ### Teste mov + sharp ### */

  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));

  robo.moveTank(LOOKING_SPEED, -LOOKING_SPEED); */

  // ### teste seguir parede ###

  //robo.follow_wall_to_cup();

  // ### teste pegar copo ###
  //robo.getTerrine();

  // ### TESTE ELEVADOR ###
  // elevador.goToStage03();
  // elevador.goToStage01();
  // elevador.goToStage02();

  // ### TESTE GARRA ###
  //braco.tryGetTerrine();
  //delay(2000);
  //braco.recolherBraco();
  //teste.write(130);



  // TESTE ESTACIONAR (LER SENSORES)
  in = ' ';

  if (Serial.available() > 0) {
    in = Serial.read();

    switch(in){
      case 'w':
        robo.moveFrente(150);
        delay(100);
        robo.moveFrente(0);
      break;

      case 's':
        robo.moveTras(150);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'a':
        robo.moveTank(-180 /* velocidade de giro do robo*/, 180 /* velocidade de giro do robo*/);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'd':
        robo.moveTank(180 /* velocidade de giro do robo*/, -180 /* velocidade de giro do robo*/);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'q':
        robo.moveTank(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/, 150);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'e':
        robo.moveTank(150, 200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'f':
        Serial.print(" S2: ");
        Serial.print(robo.getSharp(2 /* [2]*/));
        Serial.print(" S3: ");
        Serial.print(robo.getSharp(3 /* [3]*/));
        Serial.print(" S4: ");
        Serial.print(robo.getSharp(5 /* [5]*/));
        Serial.print(" S5: ");
        Serial.println(robo.getSharp(4 /* [4]*/));
      break;
    }

  }


}

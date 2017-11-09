#include "GIMu.h"
#include <LiquidCrystal.h>

Motor direito(DC11, DC12);
Motor esquerdo(DC21, DC22);

Motor mbraco(MBRACO1, MBRACO2);
BracoCopo braco(SERVOG_PULSO, SERVOG_DEDO, SH_GARRA, MSH_GARRA_D, MSH_GARRA_E, mbraco);

Motor mElevator(DC_ELEVADOR0, DC_ELEVADOR1);
Elevador elevador(mElevator, 1);

Motor sucker(DC_TIRALEITE1, DC_TIRALEITE2);

GIMu robo (direito, esquerdo, braco, elevador, sucker);

//GIMu robo (direito, esquerdo);
//GIMu robo (elevador);
//GIMu robo(braco);

Servo teste,teste1,teste2;

char in;
LiquidCrystal lcd(28,30,32,34,36,38);

void setup() {
  Serial.begin(9600);


//   robo.follow_wall_to_little_gate();
   ///raco.recolherBraco();
  //robo.follow_wall_to_terrine_area();
  //robo.adjust_to_get_cup();
  // robo.getTerrine();
// // //   robo.getTerrine();
 teste.attach(6);
// teste1.attach(44);
//  teste2.attach(46);
  /*lcd.begin(16, 2);
  lcd.print("hello, world!");*/
  //robo.ordenhar03();

  // pinMode(12, OUTPUT);
  // pinMode(11, OUTPUT);

  /* AJUSTAR PARA COPO */
  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();*/
}

void loop() {

  // robo.ordenhar04();

    // braco.tryGetTerrine();
    // braco.recolherBraco();
    
    robo.follow_wall_to_terrine_area();
    robo.adjust_to_get_cup();
    robo.getTerrine();

    //elevador.goToStage01();
    //elevador.goToStage03();
    //elevador.upToStage03();
    // digitalWrite(11, HIGH);
    // digitalWrite(12, LOW);
    // delay(4000);
    
    // digitalWrite(11, HIGH);
    // digitalWrite(12, LOW);
    // delay(4000);
    //teste.write(20);
    //teste.detach();
    //robo.taxearEsquerda();
   // robo.ordenhar03();
    //elevador.goToStage01();
    //  Serial.println(elevador.whatStage());

    //teste.write(POSICAO_INICIAL_GARRA);

    // teste.write(150);
    // teste1.write(POSICAO_INICIAL_GARRA);
    // teste2.write(POSICAO_INICIAL_PULSO);

    // braco.tryGetTerrine();
    // braco.recolherBraco();

    // braco.dropLeite();

   /*robo.getSharps();
   
   Serial.print("Frente direita: ");
   Serial.print(robo.getSharp(4));
   Serial.print("  Frente esquerda: ");
   Serial.println(robo.getSharp(3));*/

    //Serial.println(analogRead(9));

  /*braco.tryGetTerrine();
  braco.recolherBraco();*/

  /* TESTE COMPONENTE POR COMPONENTE - BRACO */

  /*Serial.print(digitalRead(FDC_FRENTE));
  Serial.print(" | ");
  Serial.println(digitalRead(FDC_TRAS));
  delay(1000);*/

  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();
  robo.getTerrine();*/

  //teste.write(0);
  
  /*mbraco.moveMotor(200, 1);
  delay(1000);*/
  /*mbraco.moveMotor(200, 0);
  delay(1000);*/
  /*teste.write(POSICAO_INICIAL_GARRA);
  Serial.println(analogRead(MSH_GARRA_D));*/

  //Serial.println(robo.getSharp(SH_GARRA));
  
    // for (unsigned a = 0; a < 90; a+=10) {
    //     teste.write(a);
    //     delay(1000);
    // }
    // for (unsigned a = 90; a >0; a-=10) {
    //     teste.write(a);
    //     delay(1000);
    // }

  //teste.write(90);
  
  /* ### Teste de Movimentação:*/
  //  robo.moveFrente(150);
  //  delay(2000);
  //  robo.moveTank(150,-150);
  //  delay(4000);
  //  robo.moveTank(150, -150);
  //  delay(100);
  //  robo.moveFrente(150);
  //  delay(300);

  //teste.write(90);

  // /* ### Teste dos sensores Sharps:*/
  // Serial.print(" S0: ");
  // Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  // Serial.print(" S1: ");
  // Serial.print(robo.getSharp(SH_DIREITA_FRENTE));
  // Serial.print("  || S2: ");
  // Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
  // Serial.print(" S3: ");
  // Serial.print(robo.getSharp(SH_FRENTE_ESQUERDA));
  // Serial.print("  || S4: ");
  // Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
  // Serial.print(" S5: ");
  // Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));
  
    // Serial.println(robo.getSharp(1));
    // Serial.print(" S0: ");
    // Serial.println(robo.getSharp(A0));
  
  //   Serial.print(" S0: ");
  //   Serial.print(robo.getSharp(0));

  // Serial.print(" S0: ");
  // Serial.println(robo.getSharp(1));

  

  
  /* ### Teste mov + sharp ### */
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));
  robo.moveTank(LOOKING_SPEED, -LOOKING_SPEED); */   

  // ### teste pegar copo ###
  //robo.getTerrine();

  // ### TESTE ELEVADOR ###
  /*elevador.goToStage03();
  elevador.goToStage02();
  elevador.goToStage03();
  elevador.goToStage01();*/

  //elevador.goToStage01();
  
  //Serial.println(elevador.whatStage());

  // ### TESTE GARRA ###
  /*braco.tryGetTerrine();
  delay(500);
  braco.recolherBraco();
  delay(2000);*/
  //teste.write(130);

  /*  */

  
  //Serial.println(robo.getSharp(SH_ORDENHADOR));
  //delay(100);
  /*lcd.setCursor(0, 1);
  lcd.print("BLA BLA BLA");*/

  

  //Serial.println(robo.getMSharp());


}

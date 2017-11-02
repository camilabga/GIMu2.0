# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h"



# 5 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 6 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 9 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 10 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 11 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2

class GIMu{
  public:
    GIMu(Motor d, Motor e);
    GIMu(BracoCopo b);
    GIMu(Motor d, Motor e, BracoCopo b);
    GIMu(Motor d, Motor e, BracoCopo b, Elevador l);
    GIMu(Elevador e);
    GIMu(Motor d, Motor e, BracoCopo b, Elevador l, Motor s);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);
    inline void stop(){moveFrente(0);}
    void taxearDireita();

    void getSharps();
    inline int getDistancia(int i){return sharpsBase[i];}
    int getSharp(int porta);
    inline int getMSharp(){return analogRead(8);}

    void follow_wall_to_terrine_area();
    void adjust_to_get_cup();
    void follow_wall_to_little_gate();

    void getTerrine();

    void ordenhar01();
    void ordenhar02();
    void ordenhar03();

    void adjust_to_derramar_leite();

    void dropMilk();
    void milkTeta();

  private:
    Motor Mright;
    Motor Mleft;
    Motor sucker;
    int sharpsBase[6];
    BracoCopo bracoCopo;
    Elevador elevador;
    Servo SM_Ordenhador;


};
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(9 /*Direita*/, 10);
Motor direito(8 /*Esquerda*/, 7);

Motor mbraco(2, 3);
BracoCopo braco(46, 44, 11, 9, 10, mbraco);

Motor mElevator(11, 12);
Elevador elevador(mElevator, 1);

Motor sucker(5, 4);

GIMu robo (direito, esquerdo, braco, elevador, sucker);

//GIMu robo (direito, esquerdo);
//GIMu robo (elevador);
//GIMu robo(braco);

Servo teste;

char in;
LiquidCrystal lcd(28,30,32,34,36,38);

void setup() {
  Serial.begin(9600);

  //robo.follow_wall_to_little_gate();

  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();
  robo.getTerrine();*/
  //teste.attach(46);
  /*lcd.begin(16, 2);
  lcd.print("hello, world!");*/
  //robo.ordenhar03();

  /* AJUSTAR PARA COPO */
  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();*/
}

void loop() {

    //Serial.println(elevador.whatStage());

    //Serial.println(analogRead(9));
  /*bool posCopo = true;
  if(posCopo){
      int distIni = 0, distFin = 0, dist = 0;
      
      // Andar para frente até encontrar o espaço entre copos
      do{
          robo.moveFrente(SEARCHING_SPEED);
      }
      while( (robo.getSharp(SH_GARRA) < TEM_COPO) 
          || (((robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2) >= 10));

      robo.stop();
      distIni = (robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2;
  
      // Andar para trás até encontrar o espaço entre copos
      do{
          robo.moveTras(SEARCHING_SPEED);
      }
      while(robo.getSharp(SH_GARRA) < TEM_COPO 
          || (((robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2) <= 30));

      robo.stop();
      distFin = (robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2;

      dist = (distIni + distFin) / 2;

      // Voltando para o centro do copo
      do{
          robo.moveFrente(SEARCHING_SPEED);
      }while(((robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2) <= dist);
      
      posCopo = false;
  }*/

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

  /*for (unsigned a = 70; a < 110; a+=10) {
    teste.write(a);
    delay(1000);
  }

  for (unsigned a = 110; a >70; a-=10) {
    teste.write(a);
    delay(1000);
  }*/

  //teste.write(90);

  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   //robo.moveTank(200, -200);*/
   //delay(2000);

  //teste.write(90);

  /* ### Teste dos sensores Sharps:*/
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(SH_DIREITA_FRENTE));
  Serial.print("  || S2: ");
  Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH_FRENTE_ESQUERDA));
  Serial.print("  || S4: ");
  Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));*/

  /* ### Teste mov + sharp ### */
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));
  robo.moveTank(LOOKING_SPEED, -LOOKING_SPEED); */

  // ### teste pegar copo ###
  //robo.getTerrine();

  // ### TESTE ELEVADOR ###
  elevador.goToStage03();
  elevador.goToStage02();
  elevador.goToStage03();
  elevador.goToStage01();

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

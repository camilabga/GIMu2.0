# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h"



# 5 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 6 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 8 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 9 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2
# 10 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.h" 2

class GIMu{
  public:
    GIMu(Motor d, Motor e);
    GIMu(BracoCopo b);
    GIMu(Motor d, Motor e, BracoCopo b);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);
    void getSharps();
    inline int getDistancia(int i){return sharpsBase[i];}

    void follow_wall_to_cup();

    int getSharp(int porta);

  private:
    Motor Mright;
    Motor Mleft;
    int sharpsBase[6];
    BracoCopo bracoCopo;
};
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(9 /*Esquerdo*/, 6);
Motor direito(5 /*Direito*/, 3);
GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(115200);
}

void loop() {
  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(200, -200);
   delay(2000);
  /* ###*/

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));*/

  /*Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
  Serial.print(" S3: ");
  Serial.println(robo.getSharp(SH_FRENTE_ESQUERDA));*/
  /*Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));*/

  // ###

  robo.follow_wall_to_cup();


}

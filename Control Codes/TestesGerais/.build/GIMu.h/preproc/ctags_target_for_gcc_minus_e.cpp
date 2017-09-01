# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.h"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.h"



# 5 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.h" 2
# 6 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.h" 2
# 7 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.h" 2

class GIMu{
  public:
    GIMu(Motor d, Motor e);
    GIMu(Motor d, Motor e, ListaSharp sensoresB);

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);

  private:
    Motor Mright;
    Motor Mleft;
    ListaSharp sensoresBase;
};
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino" 2

Motor direito(6, 9);
Motor esquerdo(3, 5);

GIMu robo (direito, esquerdo);

void setup() {

}

void loop() {
  robo.moveFrente(255);
  delay(1000);
  robo.moveTras(255);
  delay(1000);
  robo.moveTank(120, 240);
  delay(1000);
}

# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino" 2

Motor direito(6, 9);
Motor esquerdo(3, 5);
int portasSensoresBase[] = {0,1,2,3,6,7};
ListaSharp sensoresBase(6, portasSensoresBase);

GIMu robo (direito, esquerdo, sensoresBase);

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

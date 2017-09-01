# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/Motor.cpp"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/Motor.cpp"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/Motor.cpp" 2

Motor::Motor(){
  pinFrente = 13;
  pinTras = 13;
}

Motor::Motor(int f, int t){
  pinFrente = f;
  pinTras = t;
  pinMode(pinFrente, 0x1);
  pinMode(pinTras, 0x1);
}

void Motor::moveMotor(int velocidade, bool direcao){
  // direcao: 0 -> ré
        //    1 -> frente
  if (direcao) {
    analogWrite(pinFrente, velocidade);
    analogWrite(pinTras, 0);
  } else {
    analogWrite(pinFrente, 0);
    analogWrite(pinTras, velocidade);
  }
}
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

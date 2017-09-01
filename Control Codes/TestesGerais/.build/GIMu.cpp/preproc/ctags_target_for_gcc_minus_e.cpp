# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.cpp"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.cpp"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.cpp" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/GIMu.cpp" 2

GIMu::GIMu(Motor d, Motor e){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());
}

GIMu::GIMu(Motor d, Motor e, int n, SharpSensor S[]){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());

    sharpSensors = new SharpSensor[n];

    for(int i = 0; i < n; i++){
        sharpSensors[i] = S[i];
    }
}

void GIMu::moveFrente(int velocidade){
    Mright.moveMotor(velocidade, 1);
    Mleft.moveMotor(velocidade, 1);
}

void GIMu::moveTras(int velocidade){
    Mright.moveMotor(velocidade, 0);
    Mleft.moveMotor(velocidade, 0);
}

void GIMu::moveTank(int pwm_esquerdo, int pwm_direito){
    Mright.moveMotor(pwm_direito, 1);
    Mleft.moveMotor(pwm_esquerdo, 1);
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/escopo_movbasica.ino"


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

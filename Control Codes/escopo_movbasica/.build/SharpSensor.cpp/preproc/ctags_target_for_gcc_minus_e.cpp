# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/SharpSensor.cpp"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/SharpSensor.cpp"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/SharpSensor.cpp" 2

SharpSensor::SharpSensor(int porta){
    this->porta = porta;
    distancia = 0;
}

SharpSensor::SharpSensor(){
    porta = 13;
    distancia = 0;
}

void SharpSensor::getSharp(){
    const byte media = 50;
    int valueSensorAux = 0;
    int total = 0;
    int cont = 0;

    valueSensorAux = analogRead(porta);
    while (cont < media){
        total = valueSensorAux + total;
        cont++;
    }

    distancia = total / media;
}

void SharpSensor::operator=(const SharpSensor &SS){
    porta = 13;
    distancia = 0;

    porta = SS.porta;
    distancia = SS.distancia;
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

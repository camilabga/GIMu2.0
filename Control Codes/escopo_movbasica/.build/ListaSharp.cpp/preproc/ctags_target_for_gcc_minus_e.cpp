# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/ListaSharp.cpp"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/ListaSharp.cpp"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/escopo_movbasica/ListaSharp.cpp" 2

ListaSharp::ListaSharp(){
    sharps = __null;
    n = 0;
}

ListaSharp::ListaSharp(unsigned num){
    if (num != 0) {
        n = num;
        sharps = new SharpSensor[n];
    } else {
        sharps = __null;
        n = 0;
    }
}

ListaSharp::ListaSharp(unsigned tam, int portas[]){
    n = tam;
    sharps = new SharpSensor[n];

    for(unsigned i = 0; i < n; i++){
        sharps[i].setPorta(portas[i]);
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

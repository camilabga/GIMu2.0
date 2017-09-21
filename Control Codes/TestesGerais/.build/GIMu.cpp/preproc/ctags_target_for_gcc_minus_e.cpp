# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp" 2

GIMu::GIMu(Motor d, Motor e){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());
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
    if (pwm_esquerdo < 0) {
        Mleft.moveMotor(-pwm_esquerdo, 0);
    } else {
        Mleft.moveMotor(pwm_esquerdo, 1);
    }

    if (pwm_direito < 0) {
        Mright.moveMotor(-pwm_direito, 0);
    } else {
        Mright.moveMotor(pwm_direito, 1);
    }
}

int GIMu::getSharp(int porta){
    SharpIR SharpIR(porta, 1080);
    byte n = 20;
    int media, desvio;
    int x[20];
    long unsigned soma=0;
    for(int i=0;i<n;i++){
        x[i] = SharpIR.distance();
        soma += x[i]; // this returns the distance to the object you're measuring
    }
    media = (soma/n);
    soma = 0;
    for (int i =0;i< n; i++) {
      soma += (x[i]-media)*(x[i]-media);
    }

    desvio = soma/n;

    if (desvio > 35) {
      return -1; // é ruido
    } else {
      return media; // n é ruido
    }

    /*const int media = 50;
    int valueSensorAux = 0;
    int total = 0;
    int cont = 0;
    
    while (cont < media){
        total += analogRead(porta);
        cont++;
    }
    return (total / media);*/
}

void GIMu::getSharps(){
    sharpsBase[0] = getSharp(3 /* frente esquerda */);
    sharpsBase[1] = getSharp(2 /* frente direita*/);
    sharpsBase[2] = getSharp(1 /* esquerda frente*/);
    sharpsBase[3] = getSharp(0 /* esquerda tras*/);
    sharpsBase[4] = getSharp(4 /* direita frente*/);
    sharpsBase[5] = getSharp(5 /* direita tras*/);
}

void GIMu::follow_wall_to_cup() {
    bool found_wall = false;
    bool found_terrine_area = false;
    while (!found_terrine_area){
        getSharps(); // pega os valores dos sharps
        if (!found_wall){
            if ((sharpsBase[0] == -1 || sharpsBase[1] == -1) || (sharpsBase[0] >= 12 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[1] >= 12 /* distancia que identifica q o robo achou a parede*/)) {
                moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                Serial.println("Segue em frente");
            } else if (sharpsBase[0] < 12 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[1] < 12 /* distancia que identifica q o robo achou a parede*/) {
                Serial.println("Achou Parede");
                moveFrente(0);
                do {
                    getSharps();
                    moveTank(200 /* velocidade de giro do robo*/, -200 /* velocidade de giro do robo*/);
                    Serial.print(" S2: ");
                    Serial.print(sharpsBase[2]);
                    Serial.print(" S3: ");
                    Serial.println(sharpsBase[3]);
                } while(!(sharpsBase[2] != -1 || sharpsBase[3] != -1) || (((sharpsBase[2]-sharpsBase[3])>0?(sharpsBase[2]-sharpsBase[3]):-(sharpsBase[2]-sharpsBase[3])) > 5 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/));
                moveFrente(0);
                found_wall = true;
            }

        } else {
            if ((sharpsBase[0] != -1 || sharpsBase[1] != -1) && (sharpsBase[0] <= 12 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[1] <= 12 /* distancia que identifica q o robo achou a parede*/)) {
                found_terrine_area = true;
                moveFrente(0);
                Serial.println("Achei o caralho todo");
            } else {
                moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
            }
        }
    }
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(5 /*Esquerdo*/, 3);
Motor direito(9 /*Direito*/, 6);
GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(9600);
}

void loop() {
  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   robo.moveTank(120, 240);
   delay(2000);*/
  /* ###*/

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH0));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH1));*/

  /*Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH2));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH3));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH4));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH5));
  delay(500);*/
  // ###


  robo.follow_wall_to_cup();

}

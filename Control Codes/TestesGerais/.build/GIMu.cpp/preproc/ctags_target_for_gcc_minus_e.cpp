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

    if (desvio > 35 || media > 99) {
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
    sharpsBase[0] = getSharp(0 /* [0]*/);
    sharpsBase[1] = getSharp(1 /* [1]*/);
    sharpsBase[2] = getSharp(2 /* [2]*/);
    sharpsBase[3] = getSharp(3 /* [3]*/);
    sharpsBase[4] = getSharp(6 /* [5]*/);
    sharpsBase[5] = getSharp(7 /* [4]*/);
}

void GIMu::follow_wall_to_cup() {
    bool found_wall = false;
    bool found_terrine_area = false;
    while (!found_terrine_area){
        getSharps(); // pega os valores dos sharps
        if (!found_wall){
            if ((sharpsBase[2] == -1 || sharpsBase[3] == -1) || (sharpsBase[2] >= 10 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] >= 10 /* distancia que identifica q o robo achou a parede*/)) {
                moveFrente(255 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                Serial.println("Segue em frente");
            } else if (sharpsBase[2] < 10 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] < 10 /* distancia que identifica q o robo achou a parede*/) {
                Serial.println("Achou Parede");
                moveTras(255 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                delay(250);
                moveFrente(0);
                delay(250);
                do {
                    getSharps();
                    moveTank(255 /* velocidade de giro do robo*/, -255 /* velocidade de giro do robo*/);
                    Serial.print(" S2: ");
                    Serial.print(sharpsBase[4]);
                    Serial.print(" S3: ");
                    Serial.println(sharpsBase[5]);
                } while(!(sharpsBase[4] != -1 || sharpsBase[5] != -1) || (((sharpsBase[4]-sharpsBase[5])>0?(sharpsBase[4]-sharpsBase[5]):-(sharpsBase[4]-sharpsBase[5])) > 5 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/));

                found_wall = true;
                moveFrente(0);
                delay(500);
            }

        } /*else {
            if ((sharpsBase[0] != -1 || sharpsBase[1] != -1) && (sharpsBase[0] <= DIST_TURN01 || sharpsBase[1] <= DIST_TURN01)) {
                found_terrine_area = true;
                moveFrente(0);
                Serial.println("Achei o caralho todo");
            } else {
                moveFrente(LOOKING_SPEED);
            }
        }*/
    }
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(9 /*Esquerdo*/, 6);
Motor direito(5 /*Direito*/, 3);
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

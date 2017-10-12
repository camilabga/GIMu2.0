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

GIMu::GIMu(BracoCopo b){
    bracoCopo.setSharpGarra(b.getSharpGarra());
    bracoCopo.attachMotor(b.getMotor());
    bracoCopo.getPulso().attach(3);
    bracoCopo.getGarra().attach(2);
}

GIMu::GIMu(Motor d, Motor e, BracoCopo b){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());

    bracoCopo.setSharpGarra(b.getSharpGarra());
    bracoCopo.set_mSharp_D(b.get_mSharp_D());
    bracoCopo.set_mSharp_E(b.get_mSharp_E());
    bracoCopo.attachMotor(b.getMotor());
    bracoCopo.getPulso().attach(3);
    bracoCopo.getGarra().attach(2);
    bracoCopo.getPulso().write(0);
    bracoCopo.getGarra().write(130);
}

GIMu::GIMu(Motor d, Motor e, BracoCopo b, Elevador l){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());

    moveFrente(0);

    bracoCopo.setSharpGarra(b.getSharpGarra());
    bracoCopo.set_mSharp_D(b.get_mSharp_D());
    bracoCopo.set_mSharp_E(b.get_mSharp_E());
    bracoCopo.attachMotor(b.getMotor());
    bracoCopo.getPulso().attach(3);
    bracoCopo.getGarra().attach(2);
    bracoCopo.getPulso().write(0);
    bracoCopo.getGarra().write(130);

    elevador.attachMotor(l.getMotor());
    elevador.setStage(l.getStage());
}

GIMu::GIMu(Elevador e){
    elevador.attachMotor(e.getMotor());
    elevador.setStage(e.getStage());

    if (elevador.getStage() == 1) {
        elevador.goToStage01();
    } else if (elevador.getStage() == 2) {
        elevador.goToStage02();
    } else {
        elevador.goToStage03();
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

    if (desvio > 35 || media > 80) {
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
    sharpsBase[4] = getSharp(5 /* [5]*/);
    sharpsBase[5] = getSharp(4 /* [4]*/);
}

void GIMu::taxearDireita(){
    if (sharpsBase[4] > 12 && sharpsBase[5] > 12) {
        moveTank(180, 150);
    } else if (sharpsBase[4] > sharpsBase[5] + 3) {
        moveTank(180, 150);
    } else if (sharpsBase[4] + 3 < sharpsBase[5]) {
        moveTank(150, 180);
    } else if (((sharpsBase[4] - sharpsBase[5])>0?(sharpsBase[4] - sharpsBase[5]):-(sharpsBase[4] - sharpsBase[5])) < 3){
        moveTank(180, 180);
    }
}

void GIMu::follow_wall_to_cup() {
    unsigned aux = 0;
    bool found_wall = false;
    bool found_terrine_area = false;
    while (!found_terrine_area){
                   //long int init = micros();
        //getSharps(); // pega os valores dos sharps
                  //Serial.println((float)(micros() - init)/1000000);
        if (!found_wall){
            sharpsBase[2] = getSharp(2 /* [2]*/);
            sharpsBase[3] = getSharp(3 /* [3]*/);
            if ((sharpsBase[2] == -1 || sharpsBase[3] == -1) || (sharpsBase[2] >= 10 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] >= 10 /* distancia que identifica q o robo achou a parede*/)) {
                moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                Serial.println("Segue em frente");
            } else if (sharpsBase[2] < 10 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] < 10 /* distancia que identifica q o robo achou a parede*/) {
                Serial.println("Achou Parede");

                do {
                   //getSharps();
                   sharpsBase[4] = getSharp(5 /* [5]*/);
                   sharpsBase[5] = getSharp(4 /* [4]*/);
                   moveTank(180 /* velocidade de giro do robo*/, -180 /* velocidade de giro do robo*/);
                } while(!(sharpsBase[4] != -1 || sharpsBase[5] != -1) || (((sharpsBase[4]-sharpsBase[5])>0?(sharpsBase[4]-sharpsBase[5]):-(sharpsBase[4]-sharpsBase[5])) > 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/));

                found_wall = true;
                moveFrente(0);
                delay(1000);
            }

        } else {
            if (2*aux%2 + 2 < 4) {
                sharpsBase[2*aux%2 + 2] = getSharp(2 /* [2]*/);
                sharpsBase[2*aux%2 + 3] = getSharp(3 /* [3]*/);
            } else {
                sharpsBase[2*aux%2 + 2] = getSharp(5 /* [5]*/);
                sharpsBase[2*aux%2 + 3] = getSharp(4 /* [4]*/);
            }

            aux++;
            if (aux == 10) {
                aux = 0;
            }
            Serial.print(" S2: ");
            Serial.print(sharpsBase[2]);
            Serial.print(" S3: ");
            Serial.println(sharpsBase[3]);
            if ((sharpsBase[2] <= 10 /* distancia que identifica q o robo achou a parede*/ && sharpsBase[2] != -1) || (sharpsBase[3] <= 10 /* distancia que identifica q o robo achou a parede*/ && sharpsBase[3] != -1)) {
                found_terrine_area = true;
                moveFrente(0);
                Serial.println("Achei o caralho todo");
            } else {
                taxearDireita();
            }
        }
    }
}

void GIMu::getTerrine(){
    bracoCopo.tryGetTerrine();
    bracoCopo.recolherBraco();
}

void GIMu::ordenhar(){
    bool found_teta = false, found_dedo = false;
    elevador.goToStage02();
    while (!found_teta) {
        moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        if (getSharp(6) <= 300 &&
            getSharp(6) != -1) {
            found_teta = true;
            moveFrente(0);
        }
    }

    while (!found_dedo) {
        if (elevador.getStage() == 3) {
            elevador.downToStage02();
        } else if (elevador.getStage() == 2) {
            elevador.upToStage03();
        }
        if (getMSharp() <= 200) {
            found_dedo = true;
            elevador.stop();
        }
    }

}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"


Motor esquerdo(11 /*Esquerdo*/, 10);
Motor direito(8 /*Direito*/, 9);

Motor mbraco(4, 5);
BracoCopo braco(3, 2, 8, 9, 10, mbraco);

Motor mElevator(12, 13);
Elevador elevador(mElevator, 3);

GIMu robo (direito, esquerdo, braco, elevador);

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
   delay(2000);*/
  /* ###*/

  // ### Teste dos sensores Sharps:
  Serial.print(" S0: ");
  Serial.print(robo.getSharp(0 /* [0]*/));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(1 /* [1]*/));
  Serial.print(" S2: ");
  Serial.print(robo.getSharp(2 /* [2]*/));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(3 /* [3]*/));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(5 /* [5]*/));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(4 /* [4]*/));

  // ###


  /* ### Teste mov + sharp ### */

  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));

  robo.moveTank(LOOKING_SPEED, -LOOKING_SPEED); */

  // ### teste seguir parede ###

  //robo.follow_wall_to_cup();

  // ### teste pegar copo ###
  //robo.getTerrine();

  // ### TESTE ELEVADOR ###
  // elevador.goToStage03();
  // elevador.goToStage01();
  // elevador.goToStage02();

  // ### TESTE GARRA ###
  /*braco.tryGetTerrine();
  braco.recolherBraco();*/

}

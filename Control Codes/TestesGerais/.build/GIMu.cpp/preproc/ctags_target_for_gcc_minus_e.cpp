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
    bracoCopo.set_mSharp_D(b.get_mSharp_D());
    bracoCopo.set_mSharp_E(b.get_mSharp_E());
    bracoCopo.attachMotor(b.getMotor());
    /*bracoCopo.getPulso().attach(SERVOG_PULSO);
    bracoCopo.getGarra().attach(SERVOG_DEDO);    */

    pinMode(22, 0x2);
    pinMode(24, 0x2);
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
    /*bracoCopo.getPulso().attach(SERVOG_PULSO);
    bracoCopo.getGarra().attach(SERVOG_DEDO);*/
}

GIMu::GIMu(Motor d, Motor e, BracoCopo b, Elevador l){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());

    bracoCopo.setSharpGarra(b.getSharpGarra());
    bracoCopo.set_mSharp_D(b.get_mSharp_D());
    bracoCopo.set_mSharp_E(b.get_mSharp_E());
    bracoCopo.attachMotor(b.getMotor());
    /*bracoCopo.getPulso().attach(SERVOG_PULSO);
    bracoCopo.getGarra().attach(SERVOG_DEDO);*/

    elevador.attachMotor(l.getMotor());
    elevador.setStage(l.getStage());

    pinMode(22, 0x2);
    pinMode(24, 0x2);
}

GIMu::GIMu(Elevador e){
    elevador.attachMotor(e.getMotor());
    elevador.setStage(e.getStage());
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

    if (desvio > 35 || media > 35) {
        return 35; // é ruido
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
    sharpsBase[0] = getSharp(1 /* [0]*/);
    sharpsBase[1] = getSharp(2 /* [1]*/);
    sharpsBase[2] = getSharp(3 /* [2]*/);
    sharpsBase[3] = getSharp(4 /* [3]*/);
    sharpsBase[4] = getSharp(6 /* [5]*/);
    sharpsBase[5] = getSharp(5 /* [4]*/);
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
            sharpsBase[2] = getSharp(3 /* [2]*/);
            sharpsBase[3] = getSharp(4 /* [3]*/);
            if ((sharpsBase[2] == -1 || sharpsBase[3] == -1) || (sharpsBase[2] >= 10 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] >= 10 /* distancia que identifica q o robo achou a parede*/)) {
                moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                Serial.println("Segue em frente");
            } else if (sharpsBase[2] < 10 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] < 10 /* distancia que identifica q o robo achou a parede*/) {
                Serial.println("Achou Parede");

                do {
                   //getSharps();
                   sharpsBase[4] = getSharp(6 /* [5]*/);
                   sharpsBase[5] = getSharp(5 /* [4]*/);
                   moveTank(180 /* velocidade de giro do robo*/, -180 /* velocidade de giro do robo*/);
                } while(!(sharpsBase[4] != -1 || sharpsBase[5] != -1) || (((sharpsBase[4]-sharpsBase[5])>0?(sharpsBase[4]-sharpsBase[5]):-(sharpsBase[4]-sharpsBase[5])) > 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/));

                found_wall = true;
                moveFrente(0);
                delay(1000);
            }

        } else {
            if (2*aux%2 + 2 < 4) {
                sharpsBase[2*aux%2 + 2] = getSharp(3 /* [2]*/);
                sharpsBase[2*aux%2 + 3] = getSharp(4 /* [3]*/);
            } else {
                sharpsBase[2*aux%2 + 2] = getSharp(6 /* [5]*/);
                sharpsBase[2*aux%2 + 3] = getSharp(5 /* [4]*/);
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
    unsigned cont = 0;
    bool found_teta = false, found_dedo = false;
    elevador.goToStage02();
    while (!found_teta) {
        moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        if (getSharp(6) <= 300) {
            found_teta = true;
            moveFrente(0);
        }
    }

    while (!found_dedo) {
        if (cont < 3000) {
            if (elevador.getStage() == 3) {
                elevador.downToStage02();
            } else if (elevador.getStage() == 2) {
                elevador.upToStage03();
            }
            if (getMSharp() <= 200) {
                found_dedo = true;
                elevador.stop();
            }
        } else {
            found_teta = false;
            elevador.goToStage02();

            moveTank(-180 /* velocidade de giro do robo*/, 180 /* velocidade de giro do robo*/);
            delay(500);
            moveTank(180 /* velocidade de giro do robo*/, -180 /* velocidade de giro do robo*/);
            delay(700);

            cont = 0;
            found_dedo = false;
        }
        cont = (cont+1)/3000;
    }
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"


Motor esquerdo(10 /*Esquerdo*/, 9);
Motor direito(7 /*Direito*/, 8);

Motor mbraco(3, 2);
BracoCopo braco(46, 44, 11, 9, 10, mbraco);

Motor mElevator(11, 12);
Elevador elevador(mElevator, 1);

GIMu robo (direito, esquerdo, braco, elevador);

//GIMu robo (direito, esquerdo);
//GIMu robo (elevador);
//GIMu robo(braco);

//Servo teste;

char in;

void setup() {
  Serial.begin(9600);
  //teste.attach(2);
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

  /* ### Teste dos sensores Sharps:*/
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(SH_DIREITA_FRENTE));
  Serial.print("  || S2: ");
  Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH_FRENTE_ESQUERDA));
  Serial.print("  || S4: ");
  Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));*/

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
  /*elevador.goToStage03();
  elevador.goToStage01();
  elevador.goToStage02();*/

  // ### TESTE GARRA ###
  /*braco.tryGetTerrine();
  delay(500);
  braco.recolherBraco();
  delay(2000);*/
  //teste.write(130);

  /*
  // TESTE ESTACIONAR (LER SENSORES)
  in = ' ';

  if (Serial.available() > 0) {
    in = Serial.read();

    switch(in){
      case 'w':
        robo.moveFrente(PARKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 's':
        robo.moveTras(PARKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'a':
        robo.moveTank(-TURNING_SPEED, TURNING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'd':
        robo.moveTank(TURNING_SPEED, -TURNING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'q':
        robo.moveTank(LOOKING_SPEED, PARKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'e':
        robo.moveTank(PARKING_SPEED, LOOKING_SPEED);
        delay(100);
        robo.moveFrente(0);
      break;

      case 'f':
        Serial.print(" S2: ");
        Serial.print(robo.getSharp(SH_FRENTE_DIREITA));
        Serial.print(" S3: ");
        Serial.print(robo.getSharp(SH_FRENTE_ESQUERDA));
        Serial.print(" S4: ");
        Serial.print(robo.getSharp(SH_ESQUERDA_FRENTE));
        Serial.print(" S5: ");
        Serial.println(robo.getSharp(SH_ESQUERDA_TRAS));
      break;
    }

  }*/




}

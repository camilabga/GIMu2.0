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
    sharpsBase[4] = getSharp(5 /* [5]*/);
    sharpsBase[5] = getSharp(6 /* [4]*/);
}

void GIMu::taxearDireita(){
    if (sharpsBase[4] > 10 && sharpsBase[5] > 10) {
        moveTank(180, 150);
    } else if (sharpsBase[4] > sharpsBase[5] + 3) {
        moveTank(180, 150);
    } else if (sharpsBase[4] + 3 < sharpsBase[5]) {
        moveTank(150, 180);
    } else if (((sharpsBase[4] - sharpsBase[5])>0?(sharpsBase[4] - sharpsBase[5]):-(sharpsBase[4] - sharpsBase[5])) < 3){
        moveTank(180, 180);
    }
}

void GIMu::follow_wall_to_terrine_area() {
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
            if ((sharpsBase[2] == 35 || sharpsBase[3] == 35) || (sharpsBase[2] >= 9 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] >= 9 /* distancia que identifica q o robo achou a parede*/)) {
                moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                Serial.println("Segue em frente");
            } else if (sharpsBase[2] < 9 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] < 9 /* distancia que identifica q o robo achou a parede*/) {
                Serial.println("Achou Parede");

                do {
                   //getSharps();
                   sharpsBase[4] = getSharp(5 /* [5]*/);
                   sharpsBase[5] = getSharp(6 /* [4]*/);
                   moveTank(180 /* velocidade de giro do robo*/, -180 /* velocidade de giro do robo*/);
                } while(!(sharpsBase[4] != 35 || sharpsBase[5] != 35) || (((sharpsBase[4]-sharpsBase[5])>0?(sharpsBase[4]-sharpsBase[5]):-(sharpsBase[4]-sharpsBase[5])) > 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/));

                found_wall = true;
                moveFrente(0);
                delay(1000);
            }

        } else {
            if (2*aux%2 + 2 < 4) {
                sharpsBase[2*aux%2 + 2] = getSharp(3 /* [2]*/);
                sharpsBase[2*aux%2 + 3] = getSharp(4 /* [3]*/);
            } else {
                sharpsBase[2*aux%2 + 2] = getSharp(5 /* [5]*/);
                sharpsBase[2*aux%2 + 3] = getSharp(6 /* [4]*/);
            }

            aux++;

            if (aux == 10) {
                aux = 0;
            }

            if ((sharpsBase[2] <= 9 /* distancia que identifica q o robo achou a parede*/ && sharpsBase[2] != 35) || (sharpsBase[3] <= 9 /* distancia que identifica q o robo achou a parede*/ && sharpsBase[3] != 35)) {
                found_terrine_area = true;
                moveFrente(0);
                Serial.println("Achei o caralho todo");
            } else {
                moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
            }
        }
    }
}

void GIMu::adjust_to_get_cup(){
    unsigned aux = 0;
    getSharps();
    bool aligned = false;
    stop();
    delay(1000);

    do{
        Serial.println("GIRANDO");

        moveTank(-180 /* velocidade de giro do robo*/, 180 /* velocidade de giro do robo*/);

        if (aux%2 == 0) {
            sharpsBase[aux%2] = getSharp(2 /* [1]*/);
            sharpsBase[aux%2 + 1] = getSharp(1 /* [0]*/);
        } else {
            sharpsBase[aux%2 + 1] = getSharp(3 /* [2]*/);
            sharpsBase[aux%2 + 2] = getSharp(4 /* [3]*/);
        }

        Serial.print(" S0: ");
        Serial.print(sharpsBase[0]);
        Serial.print(" S1: ");
        Serial.print(sharpsBase[1]);
        Serial.print("  || S2: ");
        Serial.print(sharpsBase[2]);
        Serial.print(" S3: ");
        Serial.println(sharpsBase[3]);

        aux=(aux+1)%2;

    } while(!(sharpsBase[0] != 35 && sharpsBase[1] != 35
                && sharpsBase[2] != 35 && sharpsBase[3] != 35)
                || ((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) > 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/);

    Serial.println("POSICAO CERTA");
    stop();
    delay(1000);


    do {
        aux=0;
        unsigned reference = sharpsBase[0];
        Serial.print("reference = ");
        Serial.println(reference);

        do{
            Serial.println("APROXIMANDO");
            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(1 /* [0]*/);
                sharpsBase[aux%2 + 1] = getSharp(2 /* [1]*/);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(3 /* [2]*/);
                sharpsBase[aux%2 + 2] = getSharp(4 /* [3]*/);
            }

            Serial.print(" S0: ");
            Serial.print(sharpsBase[0]);
            Serial.print(" S1: ");
            Serial.print(sharpsBase[1]);
            Serial.print("  || S2: ");
            Serial.print(sharpsBase[2]);
            Serial.print(" S3: ");
            Serial.println(sharpsBase[3]);

            aux=(aux+1)%2;

            moveTank(-180 /* velocidade de giro do robo*/, 180 /* velocidade de giro do robo*/);
        } while (sharpsBase[0] > 9 && (((reference - sharpsBase[0])>0?(reference - sharpsBase[0]):-(reference - sharpsBase[0])) < 7));

        stop();
        delay(500);
        aux = 0;

        do {
        Serial.println("RÉ");
        if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(1 /* [0]*/);
                sharpsBase[aux%2 + 1] = getSharp(2 /* [1]*/);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(3 /* [2]*/);
                sharpsBase[aux%2 + 2] = getSharp(4 /* [3]*/);
            }

            Serial.print(" S0: ");
            Serial.print(sharpsBase[0]);
            Serial.print(" S1: ");
            Serial.print(sharpsBase[1]);
            Serial.print("  || S2: ");
            Serial.print(sharpsBase[2]);
            Serial.print(" S3: ");
            Serial.println(sharpsBase[3]);

            aux=(aux+1)%2;
            moveTras(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        } while(sharpsBase[0] > 7 && sharpsBase[2] != 35);

        stop();
        delay(500);
        aux = 0;

        do{
            Serial.println("AJUSTE");

            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(1 /* [0]*/);
                sharpsBase[aux%2 + 1] = getSharp(2 /* [1]*/);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(3 /* [2]*/);
                sharpsBase[aux%2 + 2] = getSharp(4 /* [3]*/);
            }

            Serial.print(" S0: ");
            Serial.print(sharpsBase[0]);
            Serial.print(" S1: ");
            Serial.print(sharpsBase[1]);
            Serial.print("  || S2: ");
            Serial.print(sharpsBase[2]);
            Serial.print(" S3: ");
            Serial.println(sharpsBase[3]);

            aux=(aux+1)%2;

            moveTank(180 /* velocidade de giro do robo*/, -180 /* velocidade de giro do robo*/);
        }while((((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) > 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/) && sharpsBase[2] != 35);

        if(sharpsBase[0] < 9) aligned = true;

        stop();
        delay(500);
        aux = 0;

        do {
            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(1 /* [0]*/);
                sharpsBase[aux%2 + 1] = getSharp(2 /* [1]*/);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(3 /* [2]*/);
                sharpsBase[aux%2 + 2] = getSharp(4 /* [3]*/);
            }

            Serial.print(" S0: ");
            Serial.print(sharpsBase[0]);
            Serial.print(" S1: ");
            Serial.print(sharpsBase[1]);
            Serial.print("  || S2: ");
            Serial.print(sharpsBase[2]);
            Serial.print(" S3: ");
            Serial.println(sharpsBase[3]);

            if(((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) > 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/) aligned = true;

            aux=(aux+1)%2;
            moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);

        } while(sharpsBase[2] > 12);

        stop();

        if (sharpsBase[0] - sharpsBase[1] == 0 && sharpsBase[0] == 8) {
            aligned = true;
        }

    } while(!aligned);

}

void GIMu::getTerrine(){
    bool frente = false;
    while (getSharp(11) > 20) {
        if (frente) {
            if (getSharp(3 /* [2]*/) > 10) {
                moveFrente(100);
            } else {
                frente = false;
            }
        } else {
            if (getSharp(3 /* [2]*/) < 35) {
                moveTras(100);
            } else {
                stop();
                frente = true;
            }
        }
    }

    bracoCopo.tryGetTerrine();
    bracoCopo.recolherBraco();
}

void GIMu::ordenhar01(){
    SM_Ordenhador.attach(6);
    unsigned cont = 0;
    bool found_teta = false, found_dedo = false;
    elevador.goToStage02();
    Serial.println("INICIO");
    while (!found_teta) {
      Serial.println("n achou");
        moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        Serial.println(getSharp(7));
        if (getSharp(7) <= 7) {
            found_teta = true;
            moveFrente(0);
        }
      }

    while (!found_dedo) {
      Serial.println("procurando dedo");
        if (cont < 5) {
          Serial.println("CICLE TIME");
            if (elevador.getStage() == 3) {
                elevador.downToStage02();
            } else if (elevador.getStage() == 2) {
                elevador.upToStage03();
            }
            if (getMSharp() <= 50) {
               Serial.println("achou dedo");
                found_dedo = true;
                elevador.stop();
            }
        } else {
            while(elevador.getStage() == 3) {
                elevador.downToStage02();
                if(getMSharp() < 50) {
                  found_dedo = true;
                  elevador.stop();
                  break;
                }
              }
            for(int pos = 60; pos < 120 && !found_dedo; pos+=10){
              Serial.println("muda pos");
              SM_Ordenhador.write(pos);
              while(elevador.getStage() == 2) {
                elevador.upToStage03();
                if(getMSharp() < 50) {
                  found_dedo = true;
                  elevador.stop();
                  break;
                }
              }

              while(elevador.getStage() == 3) {
                elevador.downToStage02();
                if(getMSharp() < 50) {
                  found_dedo = true;
                  elevador.stop();
                  break;
                }
              }

            }
        }
        cont++;
    }
}

void GIMu::ordenhar02(){
    SM_Ordenhador.attach(6);
    SM_Ordenhador.write(90);
    unsigned cont = 0, pos = 90;
    bool found_teta = false, found_dedo = false;
    //elevador.goToStage02();
    Serial.println("INICIO");
    while (!found_teta) {
      Serial.println("n achou");
        moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        Serial.println(getSharp(7));
        if (getSharp(7) <= 7) {
            found_teta = true;
            moveFrente(0);
        }
    }

    while (!found_dedo) {
      Serial.println("procurando dedo");
        if (cont < 5) {

            for (int pos = 60; pos < 120; pos+=5) {
              if(getMSharp() < 50) {
                    found_dedo = true;
                    elevador.stop();
                    break;
                }
                SM_Ordenhador.write(pos);
                delay(500);
            }

            for (int pos = 120; pos > 60; pos-=5) {
              if(getMSharp() < 50) {
                    found_dedo = true;
                    elevador.stop();
                    break;
                }
                SM_Ordenhador.write(pos);
                delay(500);
            }

            while(elevador.getStage() == 2) {
                if(getMSharp() < 50) {
                    found_dedo = true;
                    elevador.stop();
                    break;
                }
                elevador.downToStage01();
            }

            while(elevador.getStage() == 1) {
                if(getMSharp() < 50) {
                    found_dedo = true;
                    elevador.stop();
                    break;
                }
                elevador.upToStage02();
            }
        } else {
            break;
        }
        cont++;
    }
}

void GIMu::ordenhar03(){
    unsigned pos = 90;
    bool left = false;
    bool found_teta = false, found_dedo = false;
    SM_Ordenhador.attach(6);
    SM_Ordenhador.write(0);
    elevador.goToStage01();
    while (!found_teta) {
        moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        Serial.println(getSharp(7));
        if (getSharp(7) <= 7) {
            found_teta = true;
            moveFrente(0);
        }
    }

    while (!found_dedo) {
        while (elevador.getStage() == 1) {
            if(getMSharp() < 50) {
                found_dedo = true;
                elevador.stop();
                break;
            }
            elevador.upToStage02();
            SM_Ordenhador.write(pos);
        }

        while (elevador.getStage() == 2) {
            if(getMSharp() < 50) {
                found_dedo = true;
                elevador.stop();
                break;
            }
            elevador.downToStage01();
            SM_Ordenhador.write(pos);
        }

        if (!left) {
            if (pos+5 < 120) {
                pos = pos+5;
            } else {
                left = true;
            }
        } else {
            if (pos -5 < 60) {
                pos = pos-5;
            } else {
                left = false;
            }
        }
    }
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(9 /*Direita*/, 10);
Motor direito(8 /*Esquerda*/, 7);

Motor mbraco(2, 3);
BracoCopo braco(46, 44, 11, 9, 10, mbraco);

Motor mElevator(11, 12);
Elevador elevador(mElevator, 3);

GIMu robo (direito, esquerdo, braco, elevador);

//GIMu robo (direito, esquerdo);
//GIMu robo (elevador);
//GIMu robo(braco);

Servo teste;

char in;
LiquidCrystal lcd(28,30,32,34,36,38);

void setup() {
  Serial.begin(9600);

  //teste.attach(46);
  /*lcd.begin(16, 2);
  lcd.print("hello, world!");*/
  //robo.ordenhar03();

  /* AJUSTAR PARA COPO */
  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();*/
}

void loop() {
  braco.tryGetTerrine();
  braco.recolherBraco();

  /* TESTE COMPONENTE POR COMPONENTE - BRACO */

  /*Serial.print(digitalRead(FDC_FRENTE));
  Serial.print(" | ");
  Serial.println(digitalRead(FDC_TRAS));
  delay(1000);*/

  //teste.write(140);

  /*mbraco.moveMotor(200, 1);
  delay(1000);*/
  /*mbraco.moveMotor(200, 0);
  delay(1000);*/
  /*teste.write(POSICAO_INICIAL_GARRA);
  Serial.println(analogRead(MSH_GARRA_D));*/

  //Serial.println(robo.getSharp(SH_GARRA));

  /*for (unsigned a = 70; a < 110; a+=10) {
    teste.write(a);
    delay(1000);
  }

  for (unsigned a = 110; a >70; a-=10) {
    teste.write(a);
    delay(1000);
  }*/

  //teste.write(90);

  //Serial.println(robo.getSharp(7));

  /* ### Teste de Movimentação:*/
   /*robo.moveFrente(255);
   delay(2000);
   robo.moveTras(255);
   delay(2000);
   //robo.moveTank(200, -200);*/
   //delay(2000);

  //teste.write(90);

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

  /* ### Teste mov + sharp ### */
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH_DIREITA_TRAS));
  Serial.print(" S1: ");
  Serial.println(robo.getSharp(SH_DIREITA_FRENTE));
  robo.moveTank(LOOKING_SPEED, -LOOKING_SPEED); */

  // ### teste pegar copo ###
  //robo.getTerrine();

  // ### TESTE ELEVADOR ###
  /*elevador.goToStage03();
  elevador.goToStage01();*/

  //Serial.println(elevador.whatStage());

  // ### TESTE GARRA ###
  /*braco.tryGetTerrine();
  delay(500);
  braco.recolherBraco();
  delay(2000);*/
  //teste.write(130);

  /*  */


  //Serial.println(robo.getSharp(SH_ORDENHADOR));
  //delay(100);
  /*lcd.setCursor(0, 1);
  lcd.print("BLA BLA BLA");*/



  //Serial.println(robo.getMSharp());


}

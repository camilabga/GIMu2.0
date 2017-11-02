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

GIMu::GIMu(Motor d, Motor e, BracoCopo b, Elevador l, Motor s){
    Mright.setPinFrente(d.getPinFrente());
    Mright.setPinTras(d.getPinTras());
    Mleft.setPinFrente(e.getPinFrente());
    Mleft.setPinTras(e.getPinTras());

    bracoCopo.setSharpGarra(b.getSharpGarra());
    bracoCopo.set_mSharp_D(b.get_mSharp_D());
    bracoCopo.set_mSharp_E(b.get_mSharp_E());
    bracoCopo.attachMotor(b.getMotor());

    elevador.attachMotor(l.getMotor());
    elevador.setStage(l.getStage());

    pinMode(22, 0x2);
    pinMode(24, 0x2);

    sucker.setPinFrente(s.getPinFrente());
    sucker.setPinTras(s.getPinTras());
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
    bracoCopo.iniciar();
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
            if ((sharpsBase[2] == 35 || sharpsBase[3] == 35) ||
                (sharpsBase[2] >= 15 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] >= 15 /* distancia que identifica q o robo achou a parede*/)) {
                moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                Serial.println("Segue em frente");

            } else if (sharpsBase[2] < 15 /* distancia que identifica q o robo achou a parede*/ || sharpsBase[3] < 15 /* distancia que identifica q o robo achou a parede*/) {
                Serial.println("Achou Parede");
                aux = 0;
                do {
                    sharpsBase[4] = getSharp(5 /* [5]*/);
                    sharpsBase[5] = getSharp(6 /* [4]*/);
                    moveTank(120 /* velocidade de giro do robo*/, -120 /* velocidade de giro do robo*/);
                } while(!(sharpsBase[4] != 35 && sharpsBase[5] != 35
                    && ((sharpsBase[4]-sharpsBase[5])>0?(sharpsBase[4]-sharpsBase[5]):-(sharpsBase[4]-sharpsBase[5])) < 2));

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

            if ((sharpsBase[2] <= 8 && sharpsBase[2] != 35) || (sharpsBase[3] <= 8 && sharpsBase[3] != 35)) {
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

        moveTank(-120 /* velocidade de giro do robo*/, 120 /* velocidade de giro do robo*/);

        if (aux%3 == 0) {
            sharpsBase[aux%3] = getSharp(2 /* [1]*/);
            sharpsBase[aux%3 + 1] = getSharp(1 /* [0]*/);
        } else if (aux%3 == 1) {
            sharpsBase[aux%3 + 1] = getSharp(3 /* [2]*/);
            sharpsBase[aux%3 + 2] = getSharp(4 /* [3]*/);
        } else {
            sharpsBase[aux%3 + 2] = getSharp(5 /* [5]*/);
            sharpsBase[aux%3 + 3] = getSharp(6 /* [4]*/);
        }

        Serial.print(" S0: ");
        Serial.print(sharpsBase[0]);
        Serial.print(" S1: ");
        Serial.print(sharpsBase[1]);
        Serial.print("  || S2: ");
        Serial.print(sharpsBase[2]);
        Serial.print(" S3: ");
        Serial.print(sharpsBase[3]);
        Serial.print("  || S4: ");
        Serial.print(sharpsBase[4]);
        Serial.print(" S5: ");
        Serial.println(sharpsBase[5]);

        aux=(aux+1)%3;

    } while(!(sharpsBase[0] != 35 && sharpsBase[1] != 35 &&
        sharpsBase[2] != 35 && sharpsBase[3] != 35)
          || (((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) > 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/));

    Serial.println("POSICAO CERTA");
    stop();
    delay(1000);

        aux=0;

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

            moveTank(-120 /* velocidade de giro do robo*/, 120 /* velocidade de giro do robo*/);
        } while (sharpsBase[3] < (sharpsBase[2] + 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/));

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
        } while(sharpsBase[0] > 7 && sharpsBase[2] < 30 && sharpsBase[3] < 25);

        stop();
        delay(500);
        aux = 0;
        getSharps();
        bool frente = true;

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

            moveTank(120 /* velocidade de giro do robo*/, -120 /* velocidade de giro do robo*/);
        }while(((sharpsBase[0]-sharpsBase[1])>0?(sharpsBase[0]-sharpsBase[1]):-(sharpsBase[0]-sharpsBase[1])) > 2);

        stop();
        delay(500);
        aux = 0;

        do {
            Serial.println("Ultimo ajuste!");
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

            if (sharpsBase[2] > 30){
                stop();
                frente = true;
            } else if (sharpsBase[2] < 8) {
                stop();
                frente = false;
            }

            if(((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) < 2){
                aligned = true;
            } else {
                aligned = false;
            }

            aux=(aux+1)%2;
            if (aligned) {
                if (frente) moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
                if (!frente) moveTras(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
            } else {
                if (sharpsBase[0] > sharpsBase[1]) moveTank(-120 /* velocidade de giro do robo*/, 120 /* velocidade de giro do robo*/);
                if (sharpsBase[0] < sharpsBase[1]) moveTank(120 /* velocidade de giro do robo*/, -120 /* velocidade de giro do robo*/);
            }

        } while(!aligned || sharpsBase[2] > 12);

        stop();

}

void GIMu::getTerrine(){
    bool frente = false;
    bracoCopo.iniciar();
    Serial.println(getSharp(11));
    while (getSharp(11) > 15 /*&& getSharp(SH_FRENTE_DIREITA) < 30*/) {
        Serial.println(getSharp(11));
        if (frente) {
            if (getSharp(3 /* [2]*/) > 10) {
                moveFrente(100);
            } else {
                stop();
                frente = false;
            }
        } else {
            if (getSharp(3 /* [2]*/) < 27) {
                moveTras(100);
            } else {
                stop();
                frente = true;
            }
        }
    }

    /*bool posCopo = true;
    if(posCopo){
        int distIni = 0, distFin = 0, dist = 0;
        
        // Andar para frente até encontrar o espaço entre copos
        do{
            moveFrente(SEARCHING_SPEED);
        }
        while( (getSharp(SH_GARRA) < TEM_COPO) 
            || (((getSharp(SH_FRENTE_DIREITA) + getSharp(SH_FRENTE_ESQUERDA)) / 2) >= 10));
  
        stop();
        distIni = (getSharp(SH_FRENTE_DIREITA) + getSharp(SH_FRENTE_ESQUERDA)) / 2;
    
        // Andar para trás até encontrar o espaço entre copos
        do{
            moveTras(SEARCHING_SPEED);
        }
        while(getSharp(SH_GARRA) < TEM_COPO 
            || (((getSharp(SH_FRENTE_DIREITA) + getSharp(SH_FRENTE_ESQUERDA)) / 2) <= 30));
  
        stop();
        distFin = (getSharp(SH_FRENTE_DIREITA) + getSharp(SH_FRENTE_ESQUERDA)) / 2;
  
        dist = (distIni + distFin) / 2;
  
        // Voltando para o centro do copo
        do{
            moveFrente(SEARCHING_SPEED);
        }while(((getSharp(SH_FRENTE_DIREITA) + getSharp(SH_FRENTE_ESQUERDA)) / 2) <= dist);
        
        posCopo = false;
    }*/

    delay(250);
    stop();

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

void GIMu::follow_wall_to_little_gate() {
    bool found_wall = false;
    bool found_other_wall = false;
    bool found_porteira = false;
    bool found_porteira_frente = false;
    bool turn_left = false;
    bool turn_right = false;
    bool turned_right = false;
    unsigned aux = 0;

    getSharps();

    // Seguindo aruco ate achar uma parede ou porteira
    do{
        Serial.println("ACHAR PAREDE");

        if (aux%3 == 0) {
            sharpsBase[aux%3] = getSharp(2 /* [1]*/);
            sharpsBase[aux%3 + 1] = getSharp(1 /* [0]*/);
        } else if (aux%3 == 1) {
            sharpsBase[aux%3 + 1] = getSharp(3 /* [2]*/);
            sharpsBase[aux%3 + 2] = getSharp(4 /* [3]*/);
        } else {
            sharpsBase[aux%3 + 2] = getSharp(5 /* [5]*/);
            sharpsBase[aux%3 + 3] = getSharp(6 /* [4]*/);
        }

        aux=(aux+1)%3;

        moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
        if(sharpsBase[3] == 35 && sharpsBase[2] < 8){
            found_porteira_frente = true;
            turn_left = true;
        } else if(sharpsBase[2] == 35 && sharpsBase[3] < 8){
            found_porteira_frente = true;
            turn_right = true;
        }
    }while( (sharpsBase[2] == 35 && sharpsBase[3] == 35)
            || (sharpsBase[2] > 8 && sharpsBase[3] > 8) );

    stop();
    getSharps();

    //GIRANDO
    do{
        Serial.println("GIRANDO");

        if (aux%3 == 0) {
            sharpsBase[aux%3] = getSharp(2 /* [1]*/);
            sharpsBase[aux%3 + 1] = getSharp(1 /* [0]*/);
        } else if (aux%3 == 1) {
            sharpsBase[aux%3 + 1] = getSharp(3 /* [2]*/);
            sharpsBase[aux%3 + 2] = getSharp(4 /* [3]*/);
        } else {
            sharpsBase[aux%3 + 2] = getSharp(5 /* [5]*/);
            sharpsBase[aux%3 + 3] = getSharp(6 /* [4]*/);
        }

        aux=(aux+1)%3;

        moveTank(-120 /* velocidade de giro do robo*/, 120 /* velocidade de giro do robo*/);
    }while( !(((sharpsBase[0]-sharpsBase[1])>0?(sharpsBase[0]-sharpsBase[1]):-(sharpsBase[0]-sharpsBase[1])) < 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/ && sharpsBase[4] == 35
                && sharpsBase[5] == 35) );

            getSharps();

    //Andar ate achar a porteira ou outra parede
    do{
        Serial.println("SEGUE RETO");

        if (aux%3 == 0) {
            sharpsBase[aux%3] = getSharp(2 /* [1]*/);
            sharpsBase[aux%3 + 1] = getSharp(1 /* [0]*/);
        } else if (aux%3 == 1) {
            sharpsBase[aux%3 + 1] = getSharp(3 /* [2]*/);
            sharpsBase[aux%3 + 2] = getSharp(4 /* [3]*/);
        } else {
            sharpsBase[aux%3 + 2] = getSharp(5 /* [5]*/);
            sharpsBase[aux%3 + 3] = getSharp(6 /* [4]*/);
        }

        aux=(aux+1)%3;

        if( (sharpsBase[0] == 35 && sharpsBase[1] == 35) ){
            found_porteira = true;
            delay(2000);
        }

        if (!(sharpsBase[3] == 35 && sharpsBase[4] == 35)) {
            found_other_wall = true;
        }

        moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);

    } while(!found_porteira && !found_other_wall);


    stop();

    getSharps();

    if(found_other_wall){
        Serial.println("ACHOU QUINA");
        do{
            if (aux%3 == 0) {
                sharpsBase[aux%3] = getSharp(2 /* [1]*/);
                sharpsBase[aux%3 + 1] = getSharp(1 /* [0]*/);
            } else if (aux%3 == 1) {
                sharpsBase[aux%3 + 1] = getSharp(3 /* [2]*/);
                sharpsBase[aux%3 + 2] = getSharp(4 /* [3]*/);
            } else {
                sharpsBase[aux%3 + 2] = getSharp(5 /* [5]*/);
                sharpsBase[aux%3 + 3] = getSharp(6 /* [4]*/);
            }

            aux=(aux+1)%3;

            moveTank(120 /* velocidade de giro do robo*/, -120 /* velocidade de giro do robo*/);
        }while( !(sharpsBase[0] == 35 && sharpsBase[1] == 35 &&
                  sharpsBase[2] == 35 && sharpsBase[3] == 35 &&
                  ((sharpsBase[4]-sharpsBase[5])>0?(sharpsBase[4]-sharpsBase[5]):-(sharpsBase[4]-sharpsBase[5])) < 3 /* diferenca entre os valores de sharps q ainda serao considerados iguais*/) );

        stop();
        getSharps();

        // ir reto até achar porteira
        do {
            if (aux%3 == 0) {
                sharpsBase[aux%3] = getSharp(2 /* [1]*/);
                sharpsBase[aux%3 + 1] = getSharp(1 /* [0]*/);
            } else if (aux%3 == 1) {
                sharpsBase[aux%3 + 1] = getSharp(3 /* [2]*/);
                sharpsBase[aux%3 + 2] = getSharp(4 /* [3]*/);
            } else {
                sharpsBase[aux%3 + 2] = getSharp(5 /* [5]*/);
                sharpsBase[aux%3 + 3] = getSharp(6 /* [4]*/);
            }

            aux=(aux+1)%3;

            moveFrente(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);

            if( (sharpsBase[4] == 35 && sharpsBase[5] == 35) ){
                found_porteira = true;
                delay(2000);
            }

        } while(!found_porteira);

        stop();

        moveTank(-120 /* velocidade de giro do robo*/, 120 /* velocidade de giro do robo*/);
        delay(6000);
    }

    stop();
    getSharps();

    //Tentar ir ate o meio da porteira utilizando tempo
    if(found_porteira && !found_other_wall){
        moveTank(120 /* velocidade de giro do robo*/, -120 /* velocidade de giro do robo*/);
        delay(6000);
        stop();

        /*int time = millis();
        int timeAux = 0;

        if(turned_right){
            do{
                if (aux%3 == 0) {
                    sharpsBase[aux%3] = getSharp(SH_DIREITA_FRENTE);
                    sharpsBase[aux%3 + 1] = getSharp(SH_DIREITA_TRAS);
                } else if (aux%3 == 1) {
                    sharpsBase[aux%3 + 1] = getSharp(SH_FRENTE_DIREITA);
                    sharpsBase[aux%3 + 2] = getSharp(SH_FRENTE_ESQUERDA);
                } else {
                    sharpsBase[aux%3 + 2] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[aux%3 + 3] = getSharp(SH_ESQUERDA_TRAS);
                }
                
                aux=(aux+1)%3;

                moveFrente(LOOKING_SPEED);
            }while(sharpsBase[4] == VALID_SHARP);
            
            timeAux = millis() - time; //Tempo gasto para o comprimento da porteira
            timeAux = timeAux / 2;
            time = millis();
            stop();
            
            //Andar ate a metade da porteira
            do{
                if (aux%3 == 0) {
                    sharpsBase[aux%3] = getSharp(SH_DIREITA_FRENTE);
                    sharpsBase[aux%3 + 1] = getSharp(SH_DIREITA_TRAS);
                } else if (aux%3 == 1) {
                    sharpsBase[aux%3 + 1] = getSharp(SH_FRENTE_DIREITA);
                    sharpsBase[aux%3 + 2] = getSharp(SH_FRENTE_ESQUERDA);
                } else {
                    sharpsBase[aux%3 + 2] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[aux%3 + 3] = getSharp(SH_ESQUERDA_TRAS);
                }
                
                aux=(aux+1)%3;

                moveTras(LOOKING_SPEED);
            }while( millis() - time == timeAux);

        } else {
            do{
                if (aux%3 == 0) {
                    sharpsBase[aux%3] = getSharp(SH_DIREITA_FRENTE);
                    sharpsBase[aux%3 + 1] = getSharp(SH_DIREITA_TRAS);
                } else if (aux%3 == 1) {
                    sharpsBase[aux%3 + 1] = getSharp(SH_FRENTE_DIREITA);
                    sharpsBase[aux%3 + 2] = getSharp(SH_FRENTE_ESQUERDA);
                } else {
                    sharpsBase[aux%3 + 2] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[aux%3 + 3] = getSharp(SH_ESQUERDA_TRAS);
                }
                
                aux=(aux+1)%3;

                moveFrente(LOOKING_SPEED);
            }while( (sharpsBase[1] != VALID_SHARP && sharpsBase[0] == VALID_SHARP));
            
            timeAux = millis() - time;
            time = millis();
            stop();

            do{
                if (aux%3 == 0) {
                    sharpsBase[aux%3] = getSharp(SH_DIREITA_FRENTE);
                    sharpsBase[aux%3 + 1] = getSharp(SH_DIREITA_TRAS);
                } else if (aux%3 == 1) {
                    sharpsBase[aux%3 + 1] = getSharp(SH_FRENTE_DIREITA);
                    sharpsBase[aux%3 + 2] = getSharp(SH_FRENTE_ESQUERDA);
                } else {
                    sharpsBase[aux%3 + 2] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[aux%3 + 3] = getSharp(SH_ESQUERDA_TRAS);
                }
                
                aux=(aux+1)%3;

                moveTras(LOOKING_SPEED);
            }while( millis() - time == timeAux);
            
        }

        stop();

        //Entrar na porteira
        if(turned_right){
            do{
                if (aux%3 == 0) {
                    sharpsBase[aux%3] = getSharp(SH_DIREITA_FRENTE);
                    sharpsBase[aux%3 + 1] = getSharp(SH_DIREITA_TRAS);
                } else if (aux%3 == 1) {
                    sharpsBase[aux%3 + 1] = getSharp(SH_FRENTE_DIREITA);
                    sharpsBase[aux%3 + 2] = getSharp(SH_FRENTE_ESQUERDA);
                } else {
                    sharpsBase[aux%3 + 2] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[aux%3 + 3] = getSharp(SH_ESQUERDA_TRAS);
                }
                
                aux=(aux+1)%3;

                moveTank(-TURNING_SPEED, TURNING_SPEED);
            } while( (sharpsBase[4] != VALID_SHARP && sharpsBase[5] != VALID_SHARP) 
            && ( (sharpsBase[2] != VALID_SHARP && sharpsBase[3] != VALID_SHARP)));
        } else {
            do{
                if (aux%3 == 0) {
                    sharpsBase[aux%3] = getSharp(SH_DIREITA_FRENTE);
                    sharpsBase[aux%3 + 1] = getSharp(SH_DIREITA_TRAS);
                } else if (aux%3 == 1) {
                    sharpsBase[aux%3 + 1] = getSharp(SH_FRENTE_DIREITA);
                    sharpsBase[aux%3 + 2] = getSharp(SH_FRENTE_ESQUERDA);
                } else {
                    sharpsBase[aux%3 + 2] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[aux%3 + 3] = getSharp(SH_ESQUERDA_TRAS);
                }
                
                aux=(aux+1)%3;

                moveTank(TURNING_SPEED, -TURNING_SPEED);
            }while( (sharpsBase[0] != VALID_SHARP && sharpsBase[1] != VALID_SHARP) 
            && ( (sharpsBase[2] != VALID_SHARP && sharpsBase[3] != VALID_SHARP)));
        }

        moveFrente(LOOKING_SPEED);
        delay(1000); // Andar para passar da porteira
        stop();

        //moveTank(-TURNING_SPEED, TURNING_SPEED); //Girar ate achar o aruco do tanque
        */
    }
}

void GIMu::adjust_to_derramar_leite(){
    getSharps();

    int time = 0;
    int timeAux = 0;

    do{
        moveTank(-120 /* velocidade de giro do robo*/, 120 /* velocidade de giro do robo*/);
    }while(sharpsBase[1] == 35);

    time = millis();

    do{
        moveTank(-120 /* velocidade de giro do robo*/, 120 /* velocidade de giro do robo*/);
    }while(sharpsBase[0] == 35);

    timeAux = millis();
    time = timeAux - time;

    do{
        moveTank(120 /* velocidade de giro do robo*/, -120 /* velocidade de giro do robo*/);
    }while( millis() - timeAux < (time/2) );

    elevador.goToStage03();
}

void GIMu::dropMilk(){
    bracoCopo.dropLeite();
}

void GIMu::milkTeta(){
    sucker.moveMotor(255, 1);
    delay(10000);
    sucker.moveMotor(0,1);

    delay(500);

    moveTras(200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
    delay(2000);
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(9 /*Direita*/, 10);
Motor direito(8 /*Esquerda*/, 7);

Motor mbraco(2, 3);
BracoCopo braco(46, 44, 11, 9, 10, mbraco);

Motor mElevator(11, 12);
Elevador elevador(mElevator, 1);

Motor sucker(5, 4);

GIMu robo (direito, esquerdo, braco, elevador, sucker);

//GIMu robo (direito, esquerdo);
//GIMu robo (elevador);
//GIMu robo(braco);

Servo teste;

char in;
LiquidCrystal lcd(28,30,32,34,36,38);

void setup() {
  Serial.begin(9600);

  //robo.follow_wall_to_little_gate();

  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();
  robo.getTerrine();*/
  //teste.attach(46);
  /*lcd.begin(16, 2);
  lcd.print("hello, world!");*/
  //robo.ordenhar03();

  /* AJUSTAR PARA COPO */
  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();*/
}

void loop() {

    //Serial.println(elevador.whatStage());

    //Serial.println(analogRead(9));
  /*bool posCopo = true;
  if(posCopo){
      int distIni = 0, distFin = 0, dist = 0;
      
      // Andar para frente até encontrar o espaço entre copos
      do{
          robo.moveFrente(SEARCHING_SPEED);
      }
      while( (robo.getSharp(SH_GARRA) < TEM_COPO) 
          || (((robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2) >= 10));

      robo.stop();
      distIni = (robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2;
  
      // Andar para trás até encontrar o espaço entre copos
      do{
          robo.moveTras(SEARCHING_SPEED);
      }
      while(robo.getSharp(SH_GARRA) < TEM_COPO 
          || (((robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2) <= 30));

      robo.stop();
      distFin = (robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2;

      dist = (distIni + distFin) / 2;

      // Voltando para o centro do copo
      do{
          robo.moveFrente(SEARCHING_SPEED);
      }while(((robo.getSharp(SH_FRENTE_DIREITA) + robo.getSharp(SH_FRENTE_ESQUERDA)) / 2) <= dist);
      
      posCopo = false;
  }*/

  /*braco.tryGetTerrine();
  braco.recolherBraco();*/

  /* TESTE COMPONENTE POR COMPONENTE - BRACO */

  /*Serial.print(digitalRead(FDC_FRENTE));
  Serial.print(" | ");
  Serial.println(digitalRead(FDC_TRAS));
  delay(1000);*/

  /*robo.follow_wall_to_terrine_area();
  robo.adjust_to_get_cup();
  robo.getTerrine();*/

  //teste.write(0);

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
  elevador.goToStage03();
  elevador.goToStage02();
  elevador.goToStage03();
  elevador.goToStage01();

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

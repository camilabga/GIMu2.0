#include "Arduino.h"
#include "GIMu.h"

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

    pinMode(FDC_TRAS, INPUT_PULLUP);
    pinMode(FDC_FRENTE, INPUT_PULLUP);
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

    pinMode(FDC_TRAS, INPUT_PULLUP);
    pinMode(FDC_FRENTE, INPUT_PULLUP);
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
        soma += x[i];  // this returns the distance to the object you're measuring
    }
    media = (soma/n);
    soma = 0;
    for (int i =0;i< n; i++) {
      soma += (x[i]-media)*(x[i]-media);
    }

    desvio = soma/n;
 
    if (desvio > VALID_SHARP || media > 35) {
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
    sharpsBase[0] = getSharp(SH_DIREITA_TRAS);
    sharpsBase[1] = getSharp(SH_DIREITA_FRENTE);
    sharpsBase[2] = getSharp(SH_FRENTE_DIREITA);
    sharpsBase[3] = getSharp(SH_FRENTE_ESQUERDA);
    sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
    sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
}

void GIMu::taxearDireita(){
    if (sharpsBase[4] > DIST_TAX && sharpsBase[5] > DIST_TAX) {
        moveTank(MINOR_TAX_SPEED, MAJOR_TAX_SPEED);
    } else if (sharpsBase[4] > sharpsBase[5] + 3) {
        moveTank(MINOR_TAX_SPEED, MAJOR_TAX_SPEED);
    } else if (sharpsBase[4] + 3 < sharpsBase[5]) {
        moveTank(MAJOR_TAX_SPEED, MINOR_TAX_SPEED);
    } else if (abs(sharpsBase[4] - sharpsBase[5]) < 3){
        moveTank(MINOR_TAX_SPEED, MINOR_TAX_SPEED);
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
            sharpsBase[2] = getSharp(SH_FRENTE_DIREITA);
            sharpsBase[3] = getSharp(SH_FRENTE_ESQUERDA);
            if ((sharpsBase[2] == VALID_SHARP || sharpsBase[3] == VALID_SHARP) || 
                (sharpsBase[2] >= DIST_TURN01 || sharpsBase[3] >= DIST_TURN01)) {
                moveFrente(LOOKING_SPEED);
                Serial.println("Segue em frente");
            
            } else if (sharpsBase[2] < DIST_TURN01 || sharpsBase[3] < DIST_TURN01) {
                Serial.println("Achou Parede");
                aux = 0;
                do {
                    sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
                    moveTank(TURNING_SPEED, -TURNING_SPEED);
                } while(!(sharpsBase[4] != VALID_SHARP && sharpsBase[5] != VALID_SHARP 
                    && abs(sharpsBase[4]-sharpsBase[5]) < 2));
                
                found_wall = true;
                moveFrente(0);
                delay(1000);
            }

        } else {
            if (2*aux%2 + 2 < 4) {
                sharpsBase[2*aux%2 + 2] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[2*aux%2 + 3] = getSharp(SH_FRENTE_ESQUERDA);
            } else {
                sharpsBase[2*aux%2 + 2] = getSharp(SH_ESQUERDA_FRENTE);
                sharpsBase[2*aux%2 + 3] = getSharp(SH_ESQUERDA_TRAS);
            }
            
            aux++;

            if (aux == 10) {
                aux = 0;
            }
            
            if ((sharpsBase[2] <= DIST_TURN02 && sharpsBase[2] != VALID_SHARP) || (sharpsBase[3] <= DIST_TURN02 && sharpsBase[3] != VALID_SHARP)) {
                found_terrine_area = true;
                moveFrente(0);
                Serial.println("Achei o caralho todo");
            } else {
                moveFrente(LOOKING_SPEED);
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
    
        moveTank(-TURNING_SPEED, TURNING_SPEED);

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

    } while(!(sharpsBase[0] != VALID_SHARP && sharpsBase[1] != VALID_SHARP && 
        sharpsBase[2] != VALID_SHARP && sharpsBase[3] != VALID_SHARP)
          || (abs(sharpsBase[0] - sharpsBase[1]) > SHARP_DIFF));

    Serial.println("POSICAO CERTA");
    stop();
    delay(1000);

        aux=0;
        
        do{
            Serial.println("APROXIMANDO");
            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
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

            moveTank(-TURNING_SPEED, TURNING_SPEED);
        } while (sharpsBase[3] < (sharpsBase[2] + SHARP_DIFF));

        stop();
        delay(500);
        aux = 0;
        
        do {
        Serial.println("RÉ");
        if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
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
            moveTras(LOOKING_SPEED);
        } while(sharpsBase[0] > 8 && sharpsBase[2] != 35);

        stop();
        delay(500);
        aux = 0;
        getSharps();

        do{
            Serial.println("AJUSTE");

            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
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
            
            moveTank(TURNING_SPEED, -TURNING_SPEED);
        }while(abs(sharpsBase[0]-sharpsBase[1]) > 2);

        stop();
        delay(500);
        aux = 0;
        
        do {
            Serial.println("Ultimo ajuste!");
            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
            }

            Serial.print(" S0: ");
            Serial.print(sharpsBase[0]);
            Serial.print(" S1: ");
            Serial.print(sharpsBase[1]);
            Serial.print("  || S2: ");
            Serial.print(sharpsBase[2]);
            Serial.print(" S3: ");
            Serial.println(sharpsBase[3]);

            if(abs(sharpsBase[0] - sharpsBase[1]) < 2 && sharpsBase[0] == 8) aligned = true;

            aux=(aux+1)%2;
            if (aligned) {
                moveFrente(LOOKING_SPEED);
            } else {
                if (sharpsBase[0] > sharpsBase[1]) moveTank(-TURNING_SPEED, TURNING_SPEED);
                if (sharpsBase[0] < sharpsBase[1]) moveTank(TURNING_SPEED, -TURNING_SPEED);
            }
            
        } while(!aligned/*sharpsBase[2] > 12*/);

        stop();

        if (sharpsBase[0] - sharpsBase[1] == 0 && sharpsBase[0] == 8)  {
            aligned = true;
        }

}

void GIMu::getTerrine(){
    bool frente = false;
    bracoCopo.iniciar();
    Serial.println(getSharp(SH_GARRA));
    while (getSharp(SH_GARRA) > TEM_COPO /*&& getSharp(SH_FRENTE_DIREITA) < 30*/) {
        Serial.println(getSharp(SH_GARRA));
        if (frente) {
            if (getSharp(SH_FRENTE_DIREITA) > 10) {
                moveFrente(SEARCHING_SPEED);
            } else {
                stop();
                frente = false;
            }
        } else {
            if (getSharp(SH_FRENTE_DIREITA) < 27) {
                moveTras(SEARCHING_SPEED);
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
    SM_Ordenhador.attach(SERVO_ORDENHADOR);
    unsigned cont = 0;
    bool found_teta = false, found_dedo = false;
    elevador.goToStage02();
    Serial.println("INICIO");
    while (!found_teta) {
      Serial.println("n achou");
        moveFrente(LOOKING_SPEED);
        Serial.println(getSharp(SH_ORDENHADOR));
        if (getSharp(SH_ORDENHADOR) <= TEM_TETA) {
            found_teta = true;
            moveFrente(0);
        }
      }

    while (!found_dedo) {
      Serial.println("procurando dedo");
        if (cont < CICLE_TIME) {
          Serial.println("CICLE TIME");
            if (elevador.getStage() == 3) {
                elevador.downToStage02();
            } else if (elevador.getStage() == 2) {
                elevador.upToStage03();
            }
            if (getMSharp() <= TEM_DEDO) {  
               Serial.println("achou dedo");
                found_dedo = true;
                elevador.stop();
            }
        } else {
            while(elevador.getStage() == 3) {
                elevador.downToStage02();
                if(getMSharp() < TEM_DEDO) {
                  found_dedo = true;
                  elevador.stop();
                  break;
                }
              }
            for(int pos = ANGULO_INICIAL; pos < ANGULO_FINAL && !found_dedo; pos+=10){
              Serial.println("muda pos");
              SM_Ordenhador.write(pos);
              while(elevador.getStage() == 2) {
                elevador.upToStage03();
                if(getMSharp() < TEM_DEDO) {
                  found_dedo = true;
                  elevador.stop();
                  break;
                }
              }

              while(elevador.getStage() == 3) {
                elevador.downToStage02();
                if(getMSharp() < TEM_DEDO) {
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
    SM_Ordenhador.attach(SERVO_ORDENHADOR);
    SM_Ordenhador.write(90);
    unsigned cont = 0, pos = 90;
    bool found_teta = false, found_dedo = false;
    //elevador.goToStage02();
    Serial.println("INICIO");
    while (!found_teta) {
      Serial.println("n achou");
        moveFrente(LOOKING_SPEED);
        Serial.println(getSharp(SH_ORDENHADOR));
        if (getSharp(SH_ORDENHADOR) <= TEM_TETA) {
            found_teta = true;
            moveFrente(0);
        }
    }

    while (!found_dedo) {
      Serial.println("procurando dedo");
        if (cont < CICLE_TIME) {

            for (int pos = ANGULO_INICIAL; pos < ANGULO_FINAL; pos+=5) {
              if(getMSharp() < TEM_DEDO) {
                    found_dedo = true;
                    elevador.stop();
                    break;
                }
                SM_Ordenhador.write(pos);
                delay(500);
            }
        
            for (int pos = ANGULO_FINAL; pos > ANGULO_INICIAL; pos-=5) {
              if(getMSharp() < TEM_DEDO) {
                    found_dedo = true;
                    elevador.stop();
                    break;
                }
                SM_Ordenhador.write(pos);
                delay(500);
            }

            while(elevador.getStage() == 2) {
                if(getMSharp() < TEM_DEDO) {
                    found_dedo = true;
                    elevador.stop();
                    break;
                }
                elevador.downToStage01();
            }

            while(elevador.getStage() == 1) {
                if(getMSharp() < TEM_DEDO) {
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
    SM_Ordenhador.attach(SERVO_ORDENHADOR);
    SM_Ordenhador.write(0);
    elevador.goToStage01();
    while (!found_teta) {
        moveFrente(LOOKING_SPEED);
        Serial.println(getSharp(SH_ORDENHADOR));
        if (getSharp(SH_ORDENHADOR) <= TEM_TETA) {
            found_teta = true;
            moveFrente(0);
        }
    }

    while (!found_dedo) {
        while (elevador.getStage() == 1) {
            if(getMSharp() < TEM_DEDO) {
                found_dedo = true;
                elevador.stop();
                break;
            }
            elevador.upToStage02();
            SM_Ordenhador.write(pos);
        }

        while (elevador.getStage() == 2) {
            if(getMSharp() < TEM_DEDO) {
                found_dedo = true;
                elevador.stop();
                break;
            }
            elevador.downToStage01();
            SM_Ordenhador.write(pos);
        }

        if (!left) {
            if (pos+5 < ANGULO_FINAL) {
                pos = pos+5;
            } else {
                left = true;
            }
        } else {
            if (pos -5 < ANGULO_INICIAL) {
                pos = pos-5;
            } else {
                left = false;
            }
        }
    }
}
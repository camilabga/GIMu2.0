#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/GIMu.cpp"
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
#line 24 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void setup();
#line 37 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
void loop();
#line 24 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
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
            if ((sharpsBase[2] == VALID_SHARP || sharpsBase[3] == VALID_SHARP) || (sharpsBase[2] >= DIST_TURN01 || sharpsBase[3] >= DIST_TURN01)) {
                moveFrente(LOOKING_SPEED);
                Serial.println("Segue em frente");
            } else if (sharpsBase[2] < DIST_TURN01 || sharpsBase[3] < DIST_TURN01) {
                Serial.println("Achou Parede");

                do {
                   //getSharps();
                   sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
                   sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
                   moveTank(TURNING_SPEED, -TURNING_SPEED);
                } while(!(sharpsBase[4] != VALID_SHARP || sharpsBase[5] != VALID_SHARP) || (abs(sharpsBase[4]-sharpsBase[5]) > SHARP_DIFF));
                
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
            
            if ((sharpsBase[2] <= DIST_TURN01 && sharpsBase[2] != VALID_SHARP) || (sharpsBase[3] <= DIST_TURN01 && sharpsBase[3] != VALID_SHARP)) {
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

        if (aux%2 == 0) {
            sharpsBase[aux%2] = getSharp(SH_DIREITA_FRENTE);
            sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_TRAS);
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

    } while(!(sharpsBase[0] != VALID_SHARP && sharpsBase[1] != VALID_SHARP 
                && sharpsBase[2] != VALID_SHARP && sharpsBase[3] != VALID_SHARP)
                || abs(sharpsBase[0] - sharpsBase[1]) > SHARP_DIFF);

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
        } while (sharpsBase[0] > 9 && (abs(reference - sharpsBase[0]) < SHARP_DIFF_STOP_TURNING));

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
        } while(sharpsBase[0] > 7 && sharpsBase[2] != 35);

        stop();
        delay(500);
        aux = 0;

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
        }while((abs(sharpsBase[0] - sharpsBase[1]) > SHARP_DIFF) && sharpsBase[2] != 35);

        if(sharpsBase[0] < 9) aligned = true;

        stop();
        delay(500);
        aux = 0;
        
        do {
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

            if(abs(sharpsBase[0] - sharpsBase[1]) > SHARP_DIFF) aligned = true;

            aux=(aux+1)%2;
            moveFrente(LOOKING_SPEED);
            
        } while(sharpsBase[2] > 12);

        stop();

        if (sharpsBase[0] - sharpsBase[1] == 0 && sharpsBase[0] == 8)  {
            aligned = true;
        }

    } while(!aligned);  

}

void GIMu::getTerrine(){
    bool frente = false;
    while (getSharp(SH_GARRA) > TEM_COPO) {
        if (frente) {
            if (getSharp(SH_FRENTE_DIREITA) > 10) {
                moveFrente(SEARCHING_SPEED);
            } else {
                frente = false;
            }
        } else {
            if (getSharp(SH_FRENTE_DIREITA) < VALID_SHARP) {
                moveTras(SEARCHING_SPEED);
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
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
#include "GIMu.h"
#include <LiquidCrystal.h>

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);

Motor mbraco(MBRACO1, MBRACO2);
BracoCopo braco(SERVOG_PULSO, SERVOG_DEDO, SH_GARRA, MSH_GARRA_D, MSH_GARRA_E, mbraco);

Motor mElevator(DC_ELEVADOR1, DC_ELEVADOR0);
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


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

    pinMode(FDC_TRAS, INPUT_PULLUP);
    pinMode(FDC_FRENTE, INPUT_PULLUP);

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

void GIMu::taxearEsquerda(){
    sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
    sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
    
    if (sharpsBase[4] > DIST_TAX && sharpsBase[5] > DIST_TAX) {
        moveTank(MINOR_TAX_SPEED, MAJOR_TAX_SPEED);
    } else if (sharpsBase[4] < DIST_TAX && sharpsBase[5] < DIST_TAX) {
        moveTank(MAJOR_TAX_SPEED, MINOR_TAX_SPEED);
    } else if (sharpsBase[4] - sharpsBase[5] == SHARP_DIFF){
        moveFrente(MINOR_TAX_SPEED);
    } else if (sharpsBase[4] > sharpsBase[5]) {
        moveTank(MINOR_TAX_SPEED, MAJOR_TAX_SPEED);
    } else if (sharpsBase[4] < sharpsBase[5]) {
        moveTank(MAJOR_TAX_SPEED, MINOR_TAX_SPEED);
    } 
}

void GIMu::taxearDireita(){
    sharpsBase[0] = getSharp(SH_DIREITA_TRAS);
    sharpsBase[1] = getSharp(SH_DIREITA_FRENTE);
    
    if (sharpsBase[1] > DIST_TAX && sharpsBase[0] > DIST_TAX) {
        moveTank(MAJOR_TAX_SPEED, MINOR_TAX_SPEED);
    } else if (sharpsBase[1] < DIST_TAX && sharpsBase[0] < DIST_TAX) {
        moveTank(MINOR_TAX_SPEED, MAJOR_TAX_SPEED);
    } else if (abs(sharpsBase[1] - sharpsBase[0]) < SHARP_DIFF){
        moveFrente(MINOR_TAX_SPEED);
    } else if (sharpsBase[1] > sharpsBase[0]) {
        moveTank(MAJOR_TAX_SPEED, MINOR_TAX_SPEED);
    } else if (sharpsBase[1] < sharpsBase[0]) {
        moveTank(MINOR_TAX_SPEED, MAJOR_TAX_SPEED);
    }
}

void GIMu::taxearDireitaRe(){
    sharpsBase[0] = getSharp(SH_DIREITA_TRAS);
    sharpsBase[1] = getSharp(SH_DIREITA_FRENTE);
    
    if (sharpsBase[1] > DIST_TAX && sharpsBase[0] > DIST_TAX) {
        moveTank(-MINOR_TAX_SPEED, -MAJOR_TAX_SPEED);
    } else if (sharpsBase[1] < DIST_TAX && sharpsBase[0] < DIST_TAX) {
        moveTank(-MAJOR_TAX_SPEED,-MINOR_TAX_SPEED);
    } else if (abs(sharpsBase[1] - sharpsBase[0]) < SHARP_DIFF){
        moveTras(MINOR_TAX_SPEED);
    } else if (sharpsBase[1] > sharpsBase[0]) {
        moveTank(-MINOR_TAX_SPEED, -MAJOR_TAX_SPEED);
    } else if (sharpsBase[1] < sharpsBase[0]) {
        moveTank(-MAJOR_TAX_SPEED, -MINOR_TAX_SPEED);
    }
}

/*void GIMu::findTerrineArea(){
    follow_wall_to_terrine_area();
    adjust_to_get_cup();
    bool frente = false, found_terrine = false;
    bracoCopo.iniciar();
    unsigned i = 0;
    while (i < N_TRY) {
        Serial.println(getSharp(SH_GARRA));
        while (getSharp(SH_GARRA) > TEM_COPO && getSharp(SH_FRENTE_DIREITA) < 30) {
            Serial.println(getSharp(SH_GARRA));
            if (frente) {
                if (getSharp(SH_FRENTE_DIREITA) > 10) {
                    moveFrente(SEARCHING_SPEED);
                } else {
                    stop();
                    i++;
                    frente = false;
                }

            } else {
                if (getSharp(SH_FRENTE_DIREITA) < 27) {
                    moveTras(SEARCHING_SPEED);
                } else {
                    stop();
                    i++;
                    frente = true;
                }
            }

            if (getSharp(SH_GARRA) <= TEM_COPO) {
                found_terrine = true;
                break;
            }
        //}
    }

    delay(250);
    stop();

    if (found_terrine) {
        bracoCopo.tryGetTerrine();
        bracoCopo.recolherBraco();    
    } else {
        // GIRANDO


        //taxear esquerda
        while (sharpsBase[4] != VALID_SHARP) {
            taxearEsquerda();
        }

        while (sharpsBase[5] == VALID_SHARP ){
            
        }

    }

}*/

// ####### LADO DIREITO DA ARENA

/*void GIMu::follow_wall_to_terrine_area() {
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
                Serial.println("Segue em frente");
            
            } else if (sharpsBase[2] < DIST_TURN01 || sharpsBase[3] < DIST_TURN01) {
                Serial.println("Achou Parede");
                aux = 0;
                do {
                    sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
                    sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
                    
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
        // Serial.println("GIRANDO");
    
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

    // Serial.println("POSICAO CERTA");
    stop();
    delay(1000);

        aux=0;
        
        do{
            // Serial.println("APROXIMANDO");
            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
            }

            // Serial.print(" S0: ");
            // Serial.print(sharpsBase[0]);
            // Serial.print(" S1: ");
            // Serial.print(sharpsBase[1]);
            // Serial.print("  || S2: ");
            // Serial.print(sharpsBase[2]);
            // Serial.print(" S3: ");
            // Serial.println(sharpsBase[3]);

            aux=(aux+1)%2;

            moveTank(-TURNING_SPEED, TURNING_SPEED);
        } while (sharpsBase[3] < (sharpsBase[2] + SHARP_DIFF_BALIZA));

        stop();
        delay(500);
        aux = 0;
        
        do {
        // Serial.println("RÉ");
        if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
            }

            // Serial.print(" S0: ");
            // Serial.print(sharpsBase[0]);
            // Serial.print(" S1: ");
            // Serial.print(sharpsBase[1]);
            // Serial.print("  || S2: ");
            // Serial.print(sharpsBase[2]);
            // Serial.print(" S3: ");
            // Serial.println(sharpsBase[3]);

            aux=(aux+1)%2;
            moveTras(LOOKING_SPEED);
        } while(sharpsBase[0] > 5 && sharpsBase[2] < 30 && sharpsBase[3] < 25);

        stop();
        delay(500);
        aux = 0;
        getSharps();
        bool frente = true;

        do{
            // Serial.println("AJUSTE");

            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
            }

            // Serial.print(" S0: ");
            // Serial.print(sharpsBase[0]);
            // Serial.print(" S1: ");
            // Serial.print(sharpsBase[1]);
            // Serial.print("  || S2: ");
            // Serial.print(sharpsBase[2]);
            // Serial.print(" S3: ");
            // Serial.println(sharpsBase[3]);

            aux=(aux+1)%2;
            
            moveTank(TURNING_SPEED, -TURNING_SPEED);
        }while(abs(sharpsBase[0]-sharpsBase[1]) > 2);

        stop();
        delay(500);
        aux = 0;
        
        do {
            // Serial.println("Ultimo ajuste!");
            if (aux%2 == 0) {
                sharpsBase[aux%2] = getSharp(SH_DIREITA_TRAS);
                sharpsBase[aux%2 + 1] = getSharp(SH_DIREITA_FRENTE);
            } else {
                sharpsBase[aux%2 + 1] = getSharp(SH_FRENTE_DIREITA);
                sharpsBase[aux%2 + 2] = getSharp(SH_FRENTE_ESQUERDA);
            }

            // Serial.print(" S0: ");
            // Serial.print(sharpsBase[0]);
            // Serial.print(" S1: ");
            // Serial.print(sharpsBase[1]);
            // Serial.print("  || S2: ");
            // Serial.print(sharpsBase[2]);
            // Serial.print(" S3: ");
            // Serial.println(sharpsBase[3]);

            if (sharpsBase[2] > 30){
                stop();
                frente = true;
            } else if (sharpsBase[2] < 8) {
                stop();
                frente = false;
            }

            if(abs(sharpsBase[0] - sharpsBase[1]) < 2){
                aligned = true;
            } else {
                aligned = false;
            }

            aux=(aux+1)%2;
            if (aligned) {
                if (frente) moveFrente(LOOKING_SPEED);
                if (!frente) moveTras(LOOKING_SPEED);
            } else {
                if (sharpsBase[0] > sharpsBase[1]) moveTank(-TURNING_SPEED, TURNING_SPEED);
                if (sharpsBase[0] < sharpsBase[1]) moveTank(TURNING_SPEED, -TURNING_SPEED);
            }
            
        } while(!aligned || sharpsBase[2] > 12);

        stop();

}*/

// ###### LADO ESQUERDO ARENA

void GIMu::follow_wall_to_terrine_area() {
    bracoCopo.iniciar();
    elevador.goToStage03();
    unsigned aux = 0;
    bool found_wall = false;
    bool found_terrine_area = false;
    getSharps();
    while (!found_terrine_area){
                   //long int init = micros();
        //getSharps(); // pega os valores dos sharps
                  //Serial.println((float)(micros() - init)/1000000);

        

        if (!found_wall){
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

            if (sharpsBase[2] > DIST_TURN01 && sharpsBase[3] > DIST_TURN01) {
                moveFrente(LOOKING_SPEED);
                Serial.println("Segue em frente");
            
            } else {
                stop();
                delay(500);
                Serial.println("Achou Parede");
                aux = 0;
                getSharps();
                do {
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

                    moveTank(-TURNING_SPEED, TURNING_SPEED);
                } while(!(sharpsBase[0] != VALID_SHARP && sharpsBase[1] != VALID_SHARP 
                    && abs(sharpsBase[0]-sharpsBase[1]) < SHARP_DIFF && sharpsBase[0] < 20));
                
                found_wall = true;
                stop();
                delay(1000);
            }

        } else {
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
            
            if (sharpsBase[2] <= DIST_TURN01 || sharpsBase[3] <= DIST_TURN01 ) {
                found_terrine_area = true;
                stop();
                Serial.println("Achei o caralho todo");
                delay(500);
            } else {
                Serial.println("VAI EM FRENTE");                
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
        
        aux=(aux+1)%3;

    } while(!(sharpsBase[2] == VALID_SHARP && sharpsBase[3] == VALID_SHARP && sharpsBase[4] == VALID_SHARP
            && sharpsBase[5] == VALID_SHARP && abs(sharpsBase[0] - sharpsBase[1]) < SHARP_DIFF 
            && sharpsBase[0] != VALID_SHARP && sharpsBase[1] != VALID_SHARP));

    Serial.println("POSICAO CERTA");
    stop();
    delay(1000);

    getSharps();
        
       /* do{
            Serial.println("APROXIMANDO");
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

            // Serial.print(" S0: ");
            // Serial.print(sharpsBase[0]);
            // Serial.print(" S1: ");
            // Serial.print(sharpsBase[1]);
            // Serial.print("  || S2: ");
            // Serial.print(sharpsBase[2]);
            // Serial.print(" S3: ");
            // Serial.println(sharpsBase[3]);

            moveTank(TURNING_SPEED, -TURNING_SPEED);
        } while (sharpsBase[3] < (sharpsBase[2] + SHARP_DIFF_BALIZA));

        stop();
        delay(500);
        aux = 0;
        getSharps();

        do {
        Serial.println("RÉ");
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
        } while(sharpsBase[0] > 5 && sharpsBase[2] < 30 && sharpsBase[3] < 25);

        stop();
        delay(500);
        aux = 0;
        getSharps();
        bool frente = true;

        do{
            Serial.println("AJUSTE");

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
            // Serial.print(" S0: ");
            // Serial.print(sharpsBase[0]);
            // Serial.print(" S1: ");
            // Serial.print(sharpsBase[1]);
            // Serial.print("  || S2: ");
            // Serial.print(sharpsBase[2]);
            // Serial.print(" S3: ");
            // Serial.println(sharpsBase[3]);
            
            moveTank(-TURNING_SPEED, TURNING_SPEED);
        }while(abs(sharpsBase[4]-sharpsBase[5]) > 2);

        stop();
        delay(500);
        aux = 0;
        
        do {
            Serial.println("Ultimo ajuste!");
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

            // Serial.print(" S0: ");
            // Serial.print(sharpsBase[0]);
            // Serial.print(" S1: ");
            // Serial.print(sharpsBase[1]);
            // Serial.print("  || S2: ");
            // Serial.print(sharpsBase[2]);
            // Serial.print(" S3: ");
            // Serial.println(sharpsBase[3]);

            if (sharpsBase[2] > 30){
                stop();
                frente = true;
            } else if (sharpsBase[2] < 8) {
                stop();
                frente = false;
            }

            if(abs(sharpsBase[4] - sharpsBase[5]) < 2){
                aligned = true;
            } else {
                aligned = false;
            }

            if (aligned) {
                if (frente) moveFrente(LOOKING_SPEED);
                if (!frente) moveTras(LOOKING_SPEED);
            } else {
                if (sharpsBase[4] > sharpsBase[5]) moveTank(-TURNING_SPEED, TURNING_SPEED);
                if (sharpsBase[4] < sharpsBase[5]) moveTank(TURNING_SPEED, -TURNING_SPEED);
            }
            
        } while(!aligned || sharpsBase[2] > 12);

        stop();*/

}


void GIMu::getTerrine(){
    bool frente = false;
    bracoCopo.iniciar();
    Serial.println(getSharp(SH_GARRA));
    while (getSharp(SH_GARRA) > TEM_COPO /*&& getSharp(SH_FRENTE_DIREITA) < 30*/) {
        Serial.println(getSharp(SH_GARRA));
        if (frente) {
            if (getSharp(SH_FRENTE_DIREITA) > 10) {
                moveFrente(LOOKING_SPEED);
            } else {
                stop();
                frente = false;
            }
        } else {
            if (getSharp(SH_FRENTE_DIREITA) < 27) {
                moveTras(LOOKING_SPEED);
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

void GIMu::ordenhar04(){
    bool found_teta = false, found_dedo = false, left = false;
    unsigned pos = ANGULO_CENTRAL;
    SM_Ordenhador.attach(SERVO_ORDENHADOR);
    SM_Ordenhador.write(ANGULO_CENTRAL);
    elevador.goToStage01();

    delay(1000);
    
    do {
        if (elevador.getStage() == 1) {
            elevador.upToStage02();
        } else if (elevador.getStage() == 2) {
            elevador.downToStage01();
        }

        if (left) {
            if (pos + 2*ANGULO_VARIACAO > ANGULO_FINAL) {
                left = false;
            }
            pos = pos + ANGULO_VARIACAO;
            SM_Ordenhador.write(pos);
        } else {
            if (pos - 2*ANGULO_VARIACAO < ANGULO_INICIAL) {
                left = true;
            }
            pos = pos - ANGULO_VARIACAO;
            SM_Ordenhador.write(pos);
        }

    } while (getMSharp() > TEM_DEDO);

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
        
    }while( !(sharpsBase[2] < DIST_TURN03 || sharpsBase[3] < DIST_TURN03));

    stop();
    getSharps();
    
    //GIRANDO
    do{
        Serial.println("GIRANDO");

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
    }while( !(abs(sharpsBase[0]-sharpsBase[1]) < SHARP_DIFF && sharpsBase[4] == VALID_SHARP
                && sharpsBase[5] == VALID_SHARP && sharpsBase[1] != VALID_SHARP) );

            getSharps();

    //Andar ate achar a porteira ou outra parede
    do{
        Serial.println("SEGUE RETO");

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

        if( sharpsBase[0] == VALID_SHARP && sharpsBase[1] == VALID_SHARP &&
            sharpsBase[2] == VALID_SHARP && sharpsBase[3] == VALID_SHARP &&
            sharpsBase[4] == VALID_SHARP && sharpsBase[5] == VALID_SHARP ){
            found_porteira = true;
            Serial.println("ACHOU PORTEIRA");            
            delay(2000);
        } else if (sharpsBase[2] < DIST_TURN01 || sharpsBase[3] < DIST_TURN01) {
            found_other_wall = true;
            Serial.println("ACHOU OUTRA PAREDE");
            
        }

        taxearDireita();

    } while(!found_porteira && !found_other_wall);
    
    
    stop();

    getSharps();

    if(found_other_wall){
        do{
            Serial.println("ACHOU QUINA");
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
        }while( !(sharpsBase[0] == VALID_SHARP && sharpsBase[1] == VALID_SHARP &&
                  sharpsBase[2] == VALID_SHARP && sharpsBase[3] == VALID_SHARP && 
                  abs(sharpsBase[4]-sharpsBase[5]) < SHARP_DIFF) );
        
        stop();
        getSharps();

        // ir reto até achar porteira
        do {
            Serial.println("VAI RETO ATRAS DA PORTEIRA");
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

            taxearEsquerda();

            if( (sharpsBase[4] == VALID_SHARP && sharpsBase[5] == VALID_SHARP)
                && sharpsBase[3] == VALID_SHARP && sharpsBase[2] == VALID_SHARP
                && sharpsBase[1] == VALID_SHARP && sharpsBase[0] == VALID_SHARP ){
                Serial.println("ACHOU PORTEIRA");
                found_porteira = true;
                delay(2000);
            }

        } while(!found_porteira);
        
        stop();
        
        moveTank(-TURNING_SPEED, TURNING_SPEED);
        delay(6000);
    }

    stop();
    getSharps();
    
    //Tentar ir ate o meio da porteira utilizando tempo
    if(found_porteira && !found_other_wall){
        Serial.println("GIRANDO P FRENTE DA PORTEIRA");
        moveTank(TURNING_SPEED, -TURNING_SPEED);
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
    unsigned aux = 0;

    elevador.goToStage03();

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
    }while(sharpsBase[1] == VALID_SHARP);

    time = millis();

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
    }while(sharpsBase[0] == VALID_SHARP);
    
    timeAux = millis();
    time = timeAux - time;

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
    }while( millis() - timeAux < (time/2) );
}

void GIMu::dropMilk(){
    bracoCopo.dropLeite();
}

void GIMu::milkTeta(){
    sucker.moveMotor(255, 1);
    delay(TEMPO_DA_CHUPADA);
    sucker.moveMotor(0,1);

    delay(500);

    moveTras(LOOKING_SPEED);
    delay(2000);
}
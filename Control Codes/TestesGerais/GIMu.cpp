#include "Arduino.h"
#include "GIMu.h"

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
        Serial.print("ME: ");
        Serial.print(-pwm_esquerdo);
        Mleft.moveMotor(-pwm_esquerdo, 0);
    } else {
        Serial.print("ME: ");
        Serial.print(pwm_esquerdo);
        Mleft.moveMotor(pwm_esquerdo, 1);
    }

    if (pwm_direito < 0) {
        Serial.print(" MD: ");
        Serial.println(-pwm_direito);
        Mright.moveMotor(-pwm_direito, 0);
    } else {
        Serial.print(" MD: ");
        Serial.println(pwm_direito);
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

    if (desvio > VALID_SHARP || media > 80) {
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
    sharpsBase[0] = getSharp(SH_DIREITA_TRAS);
    sharpsBase[1] = getSharp(SH_DIREITA_FRENTE);
    sharpsBase[2] = getSharp(SH_FRENTE_DIREITA);
    sharpsBase[3] = getSharp(SH_FRENTE_ESQUERDA);
    sharpsBase[4] = getSharp(SH_ESQUERDA_FRENTE);
    sharpsBase[5] = getSharp(SH_ESQUERDA_TRAS);
}

void GIMu::follow_wall_to_cup() {
    bool found_wall = false;
    bool found_terrine_area = false;
    while (!found_terrine_area){
                   long int init = micros();
        getSharps(); // pega os valores dos sharps
                   Serial.println((float)(micros() - init)/1000000);
        if (!found_wall){
            if ((sharpsBase[2] == -1 || sharpsBase[3] == -1) || (sharpsBase[2] >= DIST_TURN01 || sharpsBase[3] >= DIST_TURN01)) {
                moveFrente(LOOKING_SPEED);
                Serial.println("Segue em frente");
            } else if (sharpsBase[2] < DIST_TURN01 || sharpsBase[3] < DIST_TURN01) {
                Serial.println("Achou Parede");
                
                moveFrente(0);
                   analogWrite(3,200);
                   analogWrite(5,0);
                   analogWrite(6,200);
                   analogWrite(9,0);
  
                do {
                   getSharps();
                   
                   // moveTras(TURNING_SPEED);
                } 
                while(1);
                //while(!(sharpsBase[4] != -1 || sharpsBase[5] != -1) || (abs(sharpsBase[4]-sharpsBase[5]) > SHARP_DIFF));







                
                found_wall = true;
                moveFrente(0);

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

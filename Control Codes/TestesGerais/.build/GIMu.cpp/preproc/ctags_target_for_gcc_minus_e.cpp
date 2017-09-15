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
    Mright.moveMotor(pwm_direito, 1);
    Mleft.moveMotor(pwm_esquerdo, 1);
}

int GIMu::getSharp(int porta){
    SharpIR SharpIR(porta, 1080);
    byte media = 10;
    long unsigned soma=0;
    for(int i=0;i<media;i++)
        soma += SharpIR.distance(); // this returns the distance to the object you're measuring
    return (soma/media);

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
    sharpsBase[0] = getSharp(0 /* frente direita */);
    sharpsBase[1] = getSharp(1 /* frente esquerda*/);
    sharpsBase[2] = getSharp(2 /* esquerda frente*/);
    sharpsBase[3] = getSharp(3 /* esquerda tras*/);
    sharpsBase[4] = getSharp(4 /* direita frente*/);
    sharpsBase[5] = getSharp(5 /* direita tras*/);
}

void GIMu::follow_wall() {
    bool found_wall = false;
    while (1){
        getSharps();
        if (!found_wall){
            if (((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) <= SHARP_DIFF &&
                (sharpsBase[0] > DIST_TURN0 && sharpsBase[1] > DIST_TURN0)) {

                moveFrente(LOOKING_SPEED);

            } else {
                if (((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) <= SHARP_DIFF) {
                    if (sharpsBase[0] > sharpsBase[1]){
                        moveTank(ADJUSTING_SPEED2, ADJUSTING_SPEED1);
                    } else {
                        moveTank(ADJUSTING_SPEED1, ADJUSTING_SPEED2);
                    }
                } else {
                    moveTras(LOOKING_SPEED);
                    delay(TEMPO_DE_RE);
                    do {
                        moveTank(TURNING_SPEED, 0);
                    } while(((sharpsBase[2] - sharpsBase[3])>0?(sharpsBase[2] - sharpsBase[3]):-(sharpsBase[2] - sharpsBase[3])) <= SHARP_DIFF);

                    found_wall = true;
                }
            }

        } else {

        }
    }
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
GIMu robo (direito, esquerdo);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // ### Teste de Movimentação:
  // robo.moveFrente(255);
  // delay(1000);
  // robo.moveTras(255);
  // delay(1000);
  // robo.moveTank(120, 240);
  // delay(1000);
  // ###

  // ### Teste dos sensores Sharps:
  Serial.print(" S0: ");
  Serial.print(robo.getSharp(0 /* frente direita */));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(1 /* frente esquerda*/));
  Serial.print(" S2: ");
  Serial.print(robo.getSharp(2 /* esquerda frente*/));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(3 /* esquerda tras*/));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(4 /* direita frente*/));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(5 /* direita tras*/));
  delay(500);
  // ###

}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/segueParede.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/segueParede.ino" 2
# 4 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/segueParede.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
GIMu robo (direito, esquerdo);

void setup() {

}

void loop() {
    robo.follow_wall();
}

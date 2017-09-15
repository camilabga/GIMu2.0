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

void GIMu::follow_wall_to_cup() {
    bool found_wall = false;
    bool found_terrine_area = false;
    while (!found_terrine_area){
        getSharps();
        if (!found_wall){
            if (sharpsBase[0] > 10 && sharpsBase[1] > 10) {
                if (((sharpsBase[0] - sharpsBase[1])>0?(sharpsBase[0] - sharpsBase[1]):-(sharpsBase[0] - sharpsBase[1])) > 5){
                    if (sharpsBase[0] > sharpsBase[1]){
                        moveTank(150, 200);
                    } else {
                        moveTank(200, 150);
                    }
                }
            } else {
                moveTras(180);
                delay(100);
                do {
                    moveTank(200, 0);
                    getSharps();
                } while(((sharpsBase[2] - sharpsBase[3])>0?(sharpsBase[2] - sharpsBase[3]):-(sharpsBase[2] - sharpsBase[3])) > 5);
            }

        } else {
            if (sharpsBase[0] > 10 && sharpsBase[1] > 10) {
                if (((sharpsBase[2] - sharpsBase[3])>0?(sharpsBase[2] - sharpsBase[3]):-(sharpsBase[2] - sharpsBase[3])) <= 5) {
                    moveFrente(180);
                    if (sharpsBase[2] > sharpsBase[3]){
                        moveTank(150, 200);
                    } else {
                        moveTank(200, 150);
                    }
                }
            } else {
                moveTras(180);
                delay(100);
                do {
                    moveTank(200, 0);
                    getSharps();
                } while(((sharpsBase[2] - sharpsBase[3])>0?(sharpsBase[2] - sharpsBase[3]):-(sharpsBase[2] - sharpsBase[3])) > 5);
                found_terrine_area = true;
            }
        }
    }
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"

# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
GIMu robo (direito, esquerdo);

void setup() {
  //Serial.begin(9600);
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
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH0));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(SH1));
  Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH2));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH3));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH4));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH5));
  delay(500);*/
  // ###


  robo.follow_wall_to_cup();

}

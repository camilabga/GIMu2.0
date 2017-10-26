#include <Wire.h>
#include "GIMu.h"
#include "Pins.cpp"

#define SLAVE_ADDRESS 0x05
#define BYTES 10
// TEste
char in[BYTES*4];
char out[BYTES] = {"Falei...;"};
char sha[BYTES] = {"Falei...;"};
int qtdErro = 0;
unsigned long tempo, tempo2;
bool flag = false, flag2 = false;

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

void setup() {
  Wire.setTimeout(10);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.begin(9600);
}

void loop() {
  if(flag){
    tempo = millis();
    flag = false;
  }else{
    if((millis() - tempo) > 1000 && flag2){  
      robo.moveTank(0,0);
      Serial.println("Parando por inatividade..");
      flag2 = false;
    }
  }

  // delay(250);
  // tempo2 = millis();
  
  // Serial.println(millis() - tempo2);
}

void receiveData(int byteCount) {
  if(byteCount != BYTES){
    while(Wire.available()) {
      Wire.read();
    }
    Serial.println("Erro contagem de bytes!");
  }else{
    if(Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
    }

    switch(in[0]){
      case 'F':
        Serial.println("F");  
        robo.moveFrente(150);
        flag = true;flag2 = true;
        for(int i=0;i<6;i++){
          out[i] = 0;
        }
      break;
      case 'T':
        Serial.println("T");
        robo.moveTras(150);
        flag = true;flag2 = true;
        for(int i=0;i<6;i++){
          out[i] = 0;
        }
      break;
      case 'D':
        Serial.println("D");
        robo.moveTank(150,-150);
        flag = true;flag2 = true;
        for(int i=0;i<6;i++){
          out[i] = 0;
        }
      break;
      case 'E':
        Serial.println("E");
        robo.moveTank(-150,150);
        flag = true;flag2 = true;
        for(int i=0;i<6;i++){
          out[i] = 0;
        }
      break;
      case 'I':
        
        sha[0] = 35;
        sha[1] = 35;
        sha[2] = (char)robo.getSharp(SH_FRENTE_DIREITA);
        sha[3] = (char)robo.getSharp(SH_FRENTE_ESQUERDA);
        sha[4] = (char)robo.getSharp(SH_ESQUERDA_FRENTE);
        sha[5] = (char)robo.getSharp(SH_ESQUERDA_TRAS);
        for(int i=0;i<6;i++){
          out[i] = sha[i];
        }
        // Serial.println("Info.....");
      break;
      case 'M':
        Serial.print("Dados: ");
        Serial.print((int)in[1]);
        Serial.print(" ");
        Serial.println((int)in[2]);
        robo.moveTank(((int)in[1])*2, ((int)in[2])*2);
        flag = true;flag2 = true;
        // Serial.println("Movendo..");
      break;
      case 'S':
        robo.moveTank(0,0);
        // Serial.println("Parando..");
      break;
      default:

      break;
    }
  }
}

void sendData() {
  out[9] = ';';
  Wire.write(out, BYTES);
}
//
#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino"
//#include <Wire.h>
#include "GIMu.h"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

#define SLAVE_ADDRESS 0x04
#define BYTES 10

unsigned char S[10];

/*char in[BYTES*4];
char out[BYTES] = {"01234567;"};
int qtdErro = 0;*/

#line 17 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino"
void setup();
#line 26 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino"
void loop();
#line 17 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino"
void setup() {
  Serial.begin(9600);
  /*Wire.setTimeout(10);
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);*/
}

void loop() {
  /*receiveData(BYTES);
  sendData();
  delay(10);*/

  Serial.readBytesUntil(';', S, 7);
  if(S[0]=='r'){
      int velDir = map((int)S[1],100,200,LOOKING_SPEED,255);
      int velEsq = LOOKING_SPEED;

      robo.moveTank(velEsq, velDir);

  } else if (S[0]=='f') {
    robo.moveTank(-LOOKING_SPEED, LOOKING_SPEED);

  } else if (S[0]=='p') {
    robo.moveFrente(0);
  }

  
}

/*void receiveData(int byteCount) {
  if(byteCount != BYTES){
    while(Wire.available()) {
      Wire.read();
    }
    qtdErro++;
    Serial.print("Erro : ");
    Serial.print(qtdErro);
    Serial.print(" - Qtd Bytes: ");
    Serial.println(byteCount);
  }else{
    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
      Serial.println(in);
      if (in[0] == 1) {
        robo.moveTank(in[1], in[2]);
        Serial.println("mexe p um lado");
      } else if (in[0] == 0) {
        Serial.println("para");
        robo.moveFrente(0);
      } else if (in[0] == 2) {
        Serial.println("procura");
        robo.moveTank(-LOOKING_SPEED, LOOKING_SPEED);
      }
    }
    
    for(int i=0;i<BYTES*4;i++){
      in[i] = '\0';
    }
  }
}

void sendData() {
  Wire.write(out, BYTES);
}*/


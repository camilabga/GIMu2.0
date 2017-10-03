#include <Arduino.h>
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
#line 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
#include <Wire.h>
#include "GIMu.h"

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

#define SLAVE_ADDRESS 0x04
#define BYTES 10

unsigned char in[BYTES*4];
unsigned char out[BYTES] = {"Falei....;"};

/*char in[BYTES*4];
char out[BYTES] = {"01234567;"};
int qtdErro = 0;*/

#line 18 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
void setup();
#line 28 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
void loop();
#line 52 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
void receiveData(int byteCount);
#line 84 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
void sendData();
#line 18 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
void setup() {
  Serial.begin(9600);
  Wire.setTimeout(10);
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

}

void loop() {
  /*receiveData(BYTES);
  sendData();
  delay(10);*/

  /*Serial.readBytesUntil(';', S, 7);
  if(S[0]=='r'){
      int velDir = 100;
      int velEsq = (int)S[1];
      Serial.print(velEsq);
      Serial.print(" ");
      Serial.println(velDir);
      robo.moveTank(velEsq, velDir);

  } else if (S[0]=='f') {
    robo.moveTank(-100, 100);
    Serial.println("girar");

  } else if (S[0]=='p') {
    robo.moveFrente(0);
    Serial.println("parado");
  }*/
}

void receiveData(int byteCount) {
  if(byteCount != BYTES){
    while(Wire.available()) {
      Wire.read();
    }
  }else{
    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
      if(in[0]=='r'){
          int velDir = 100;
          int velEsq = (int)in[1];
          Serial.print(velEsq);
          Serial.print(" ");
          Serial.println(velDir);
          robo.moveTank(velEsq, velDir);
    
      } else if (in[0]=='f') {
        robo.moveTank(-100, 100);
        Serial.println("girar");
    
      } else if (in[0]=='p') {
        robo.moveFrente(0);
        Serial.println("parado");
      }
    }
    
    for(int i=0;i<BYTES*4;i++){
      in[i] = '\0';
    }
  }
}

void sendData() {
  Wire.write(out, BYTES);
}

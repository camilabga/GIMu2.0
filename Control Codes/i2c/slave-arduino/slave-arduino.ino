#include <Wire.h>
#include "GIMu.h"
#include "Pins.cpp"

#define SLAVE_ADDRESS 0x05
#define BYTES 10
// TEste
char in[BYTES*4];
char out[BYTES] = {"Falei....;"};
int qtdErro = 0;
unsigned long tempo;
bool flag = false;
unsigned long tempo2;

Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
GIMu robo (direito, esquerdo);

//Delay sem ser delay:
void delay2(int milsec){
  unsigned int tempo;
  while((millis() - tempo) < milsec){  }
}

void setup() {
  Wire.setTimeout(10);
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.begin(9600);
}

void loop() {
  // if(flag){
  //   tempo = millis();
  //   while((millis() - tempo) <100){  }
  //   robo.moveFrente(0);
  //   flag = false;
  //  }
  //  tempo2 = millis();
   out[0] = 35;
   out[1] = 35;
   out[2] = robo.getSharp(SH_FRENTE_DIREITA);
   out[3] = robo.getSharp(SH_FRENTE_ESQUERDA);
   out[4] = robo.getSharp(SH_ESQUERDA_FRENTE);
   out[5] = robo.getSharp(SH_ESQUERDA_TRAS);
  //  Serial.println(millis()- tempo2);
   
}

void receiveData(int byteCount) {
  if(byteCount != BYTES){
    while(Wire.available()) {
      Wire.read();
    }
    qtdErro++;
    // Serial.print("Erro : ");
    // Serial.print(qtdErro);
    // Serial.print(" - Qtd Bytes: ");
    // Serial.println(byteCount);
  }else{
    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
    }

     
     switch(in[0]){
      // case 'F':
      //   Serial.println("F");  
      //   robo.moveFrente(150);
      //   flag = true;
      //   for(int i=0;i<6;i++){
      //     out[i] = 0;
      //   }
      // break;
      // case 'T':
      //   Serial.println("T");
      //   robo.moveTras(150);
      //   flag = true;
      //   for(int i=0;i<6;i++){
      //     out[i] = 0;
      //   }
      // break;
      // case 'D':
      //   Serial.println("D");
      //   robo.moveTank(150,-150);
      //   flag = true;
      //   for(int i=0;i<6;i++){
      //     out[i] = 0;
      //   }
      // break;
      // case 'E':
      //   Serial.println("E");
      //   robo.moveTank(-150,150);
      //   flag = true;
      //   for(int i=0;i<6;i++){
      //     out[i] = 0;
      //   }
      // break;
       case 'I':
          
         robo.moveTank(0,0);
       break;
       case 'M':
       Serial.print((int)in[1]);
       Serial.print(" ");
       Serial.println((int)in[2]);
          robo.moveTank(((int)in[1])*2, ((int)in[2])*2);
          // flag = true;
       break;
       default:
       
       break;
     }
  }
}

void sendData() {
  Wire.write(out, BYTES); 
}

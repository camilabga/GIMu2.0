#include <Wire.h>
// #include "GIMu.h"
// #include "Pins.cpp"

//Definicoes I2C:
#define SLAVE_ADDRESS 0x04
#define QTD_BYTES_I2C 10
char in[QTD_BYTES_I2C*4];
char out[QTD_BYTES_I2C];
char msgPadrao[QTD_BYTES_I2C] = {".........;"};
//

//Outras definicoes:
bool flag = false;
//

//Delay sem ser delay:
void delay2(int milsec){
  unsigned int tempo;
  while((millis() - tempo) < milsec){  }
}
//

void setup() {
  //I2C:
  Wire.setTimeout(10);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  //

  Serial.begin(9600);
}

void loop() {
  if(flag){
    delay2(1000);
    flag = false;
  }
}

void receiveData(int byteCount) {
  if(byteCount != QTD_BYTES_I2C){
    while(Wire.available()) {
      Wire.read();
    }
  }else{
    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
    }

    switch(in[0]){
      
      case 'I':
        robo.getSharps();
        for(int i=0;i<6;i++){
          // out[i] = robo.sharpsBase[i];
        }
        // robo.moveTank(0,0);
      break;
      default:
        
      break;
    }
  }
}

void sendData() {
  Wire.write(out, QTD_BYTES_I2C);
}

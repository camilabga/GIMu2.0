#include <Wire.h>


#define SLAVE_ADDRESS 0x04
#define BYTES 10

char in[BYTES*4];
char out[BYTES] = {"Falei....;"};
int qtdErro = 0;

void setup() {
  Wire.setTimeout(10);
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.begin(9600);
}

void loop() {
  delay(10);
}

void receiveData(int byteCount) {
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
    }
//    Serial.println(in);
//    for(int i=0;i<BYTES*4;i++){
//      in[i] = '\0';
//    }
  }
}

void sendData() {
  Wire.write(out, BYTES);
}

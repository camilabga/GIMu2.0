#include <Wire.h>

//Definicoes I2C:
#define SLAVE_ADDRESS 0x04
#define QTD_BYTES_I2C 10
byte in[QTD_BYTES_I2C*4];
byte out[QTD_BYTES_I2C] = {".........;"};
byte msgPadrao[QTD_BYTES_I2C] = {".........;"};
void clearBuf(){
  for (int i=0; i<QTD_BYTES_I2C; i++) {
    in[i]='\0';
    out[i]=msgPadrao[i];
  }
}
//

//Definicoes estados:
int estadoAtual = 0, estadoAntigo = 0;
//

//Outras definicoes:
// bool flag = false;
bool erroCom = false;
//

//Delay sem ser delay:
void delay2(int milsec){
  unsigned int tempo;
  while((millis() - tempo) < milsec){  }
}
//

void setup() {
  //I2C:
  Wire.setTimeout(100);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  //

  //Serial:
  Serial.begin(9600);
  //
}

void loop() {
  // if(flag){
  //   delay2(1000);
  //   flag = false;
  // }
}

void receiveData(int byteCount) {
  if(byteCount != QTD_BYTES_I2C){
    Serial.println("Erro!, Msg descartada!");
    while(Wire.available()) {
      Wire.read();
    }
    clearBuf();
    erroCom = true;
  }else{
    clearBuf();
    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
    }
    Serial.print("Recebido: ");
    Serial.println((int)in[0]); 
    switch(in[0]){
      case 1:
        //Segue Parede:
        estadoAntigo = estadoAtual;
        estadoAtual = 1;

      break;
      case 2:
        //Procura Copo:
        estadoAntigo = estadoAtual;
        estadoAtual = 2;

      break;
      case 3:
        //Pega Copo:
        estadoAntigo = estadoAtual;
        estadoAtual = 3;

      break;
      case 'V':
        out[0] = 'T';
        out[1] = '0';
        out[2] = '1';
      break;
      case 'D':
        out[0] = 'T';
        out[1] = '0';
        out[2] = '2';
      break;
      default:
          
      break;
    }
  }
}

void sendData() {
  if(erroCom){
    Wire.write(msgPadrao, QTD_BYTES_I2C);
  }else{
    Wire.write(out, QTD_BYTES_I2C);
  }
}

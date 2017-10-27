#include <Wire.h>

//Definicoes I2C:
#define SLAVE_ADDRESS 0x05
#define QTD_BYTES_I2C 10
char in[QTD_BYTES_I2C*4];
char out[QTD_BYTES_I2C] = {".........;"};
char msgPadrao[QTD_BYTES_I2C] = {".........;"};

void clearBuf(){
  for (int i=0; i<QTD_BYTES_I2C; i++) {
    in[i]='\0';
    out[i]=msgPadrao[i];
  }
}
//

//Definicoes estados:
int estadoAtual = 0;
bool fimE1=true;
bool fimE2=true;
//

//Outras definicoes:
bool flag = false, flag2 = false;
bool erroCom = false;
unsigned long tempo;
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
  if(flag){
    tempo = millis();
    flag = false;
  }else{
    if((millis() - tempo) > 1000 && flag2){  
      // robo.moveTank(0,0);
      Serial.println("Parando por inatividade..");
      flag2 = false;
    }
  }
  switch(estadoAtual){
    case 1:
      //Executa funcao segue parede
      delay(3000);
      //
      
      fimE1 = true;
      estadoAtual = 90;
    break;
    case
      //Executa funcao 
  }

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
    Serial.print(in[0]);
    Serial.print(" ");
    Serial.println(in[1]); 
    switch(in[0]){
      //Segue Parede:
      case 1:
        out[0]=1;
        
        switch(in[1]){
          
          case 1://Mandando arduino começar.
            estadoAtual = 1;
            out[1] = 1;
            fimE1 = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimE1){
              out[3] = 1;
            }else{
              out[3] = 2;
            }
          break;
          
          default:
            out[0] = 98;
          break;
        }
      break;
      //Procura Copo:
      case 2:
        

      break;
      case 3:
        //Pega Copo:

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

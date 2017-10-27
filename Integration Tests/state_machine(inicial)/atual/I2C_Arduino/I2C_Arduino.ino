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
#define QTD_ESTADOS 10
int estadoAtual = 0;
bool fimEstado[10];
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

  //Inicializando variaveis:
  for(int i=0; i<QTD_ESTADOS;i++){
    fimEstado[i] = true;
  }
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
      
      //Fim estado:
      fimEstado[1] = true;
      estadoAtual = 90;
      //

    break;

    case 2:
    
      //Executa funcao procura copo
      delay(3000);
      //
      
      //Fim estado:
      fimEstado[2] = true;
      estadoAtual = 90;
      //
    
    break;

    case 3:
    
      //Executa funcao pega copo
      delay(3000);
      //
      
      //Fim estado:
      fimEstado[3] = true;
      estadoAtual = 90;
      //
    
    break;

    case 4:
    
      //Executa funcao segue vaca
      //delay(3000);
      //
      
    break;

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
      
      //  ####  Segue parede  #### 
      case 1:
        out[0]=1;
        
        switch(in[1]){
          
          case 1://Mandando arduino começar.
            estadoAtual = 1;
            out[1] = 1;
            fimEstado[1] = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimEstado[1]){
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
      //  ####  
      
      //  ####  Procura copo  #### 
      case 2:
      out[0]=2;
      
      switch(in[1]){
        
        case 1://Mandando arduino começar.
          estadoAtual = 2;
          out[1] = 1;
          fimEstado[2] = false;
        break;

        case 2://Perguntando se ja terminou.
          out[1] = 2;
          if(fimEstado[2]){
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
      //  ####

      //  ####  Pega Copo  #### 
      case 3:
      out[0]=3;
      
      switch(in[1]){
        
        case 1://Mandando arduino começar.
          estadoAtual = 3;
          out[1] = 1;
          fimEstado[3] = false;
        break;

        case 2://Perguntando se ja terminou.
          out[1] = 2;
          if(fimEstado[3]){
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
      //  ####

      //  ####  Procura/Segue vaca  #### 
      case 4:
        out[0]=4;
        
        switch(in[1]){
          
          case 1://Mandando arduino começar.
            estadoAtual = 4;
            out[1] = 1;
            fimEstado[4] = false;
          break;

          // case 2://Perguntando se ja terminou.
          //   out[1] = 2;
          //   if(fimEstado[4]){
          //     out[3] = 1;
          //   }else{
          //     out[3] = 2;
          //   }
          // break;

          case 2: //Recebendo comando 
            if(in[3]=='r'){
              int velDir = 100;
              int velEsq = (int)in[4];
              Serial.print(velEsq);
              Serial.print(" ");
              Serial.println(velDir);
              robo.moveTank(velEsq, velDir);
      
            } else if (in[3]=='f') {
              robo.moveTank(-100, 100);
              Serial.println("girar");
      
            } else if (in[3]=='p') {
              robo.moveFrente(0);
              Serial.println("parado");
            }
          break;

          default:
            out[0] = 98;
          break;

        }

      break;
      //  ####

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

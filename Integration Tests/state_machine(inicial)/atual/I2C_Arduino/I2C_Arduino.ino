#include <Wire.h>
#include "GIMu.h"
//

//Definições Robô:
Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);

Motor mbraco(MBRACO1, MBRACO2);
BracoCopo braco(SERVOG_PULSO, SERVOG_DEDO, SH_GARRA, MSH_GARRA_D, MSH_GARRA_E, mbraco);

Motor mElevator(DC_ELEVADOR1, DC_ELEVADOR0);
Elevador elevador(mElevator, 3);

GIMu robo (direito, esquerdo, braco, elevador);
//

//Definicoes I2C:
#define SLAVE_ADDRESS 0x05
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
#define QTD_ESTADOS 10
int estadoAtual = 90;
int subEstado = 90;
bool fimEstado[10];
//

//Definições vaca:
byte ladoV;
byte anguloV;
bool manobrandoV = false;
bool emFrenteV = false;
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
      switch(subEstado){
        case 1:
          //GIRAR PARA O LADO:
          Serial.println("Girando para o lado");
        break;

        case 2:
          //MANOBRAR
          Serial.print("Manobrando ...");
          Serial.print(ladoV);
          Serial.print(" ");
          Serial.println(anguloV);
          delay(3000);
          Serial.println("Finalizado Manobra!");
          subEstado = 90;
        break;

        case 3:
          //IR EM FRENTE ATÉ A VACA:
          Serial.println("Indo em frente..");
          delay(2000);
          Serial.println("Parei de ir pra frente");
          subEstado = 90;
        break;

        case 90:
          //MOTOR PARA
          Serial.println("PAREI !");
        break;
      }
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

    if(in[9] != ';')  out[0] = 99;
    else switch(in[0]){
      
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
          
          case 1://Inicio estado 4
            out[1] = 1;
            estadoAtual = 4;
          break;

          case 2: 
            out[1] = 2;
            if(in[3] == 1){
              subEstado = 1;
              out[3] = 1;
              flag = true;flag2 = true;
            }else if(in[3] == 2){
              subEstado = 90;
              out[3] = 2;
            }else{
              out[0] = 98;
            }
          break;

          case 3:
            out[1] = 3;
            if(in[3] == 1 || in[3] == 2){
              ladoV = in[3];
              anguloV = in[4];
              subEstado = 2;
            }else{
              out[0] = 98;
            }
          break;

          case 4:
            out[1] = 4;
            if(subEstado == 90){
              out[3] = 1;
            }else{
              out[3] = 2;
            }
          break;

          case 5:
            out[1] = 5;
            subEstado = 3;
          break;

          case 6:
            out[1] = 6;
            if(subEstado == 90){
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

      default:
        out[0] = 98;
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

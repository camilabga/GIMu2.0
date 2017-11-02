#include <Wire.h>
#include "GIMu.h"
#include <LiquidCrystal.h>

// ### Definições Robô
Motor esquerdo(DC11, DC12);
Motor direito(DC21, DC22);
Motor mbraco(MBRACO1, MBRACO2);
BracoCopo braco(SERVOG_PULSO, SERVOG_DEDO, SH_GARRA, MSH_GARRA_D, MSH_GARRA_E, mbraco);
Motor mElevator(DC_ELEVADOR1, DC_ELEVADOR0);
Elevador elevador(mElevator, 3);
GIMu robo (direito, esquerdo, braco, elevador);

// ### Definicoes I2C:
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

//Definicoes estados:
#define QTD_ESTADOS 10
int estadoAtual = 90;
int subEstado = 90;
bool fimEstado[10];

//Definições vaca:
#define ADJ_DEG 20
#define ADJ_FOR 110
#define TIME_90 4000
#define VEL_ROB 150
byte ladoV;
byte anguloV;
bool manobrandoV = false;
bool emFrenteV = false;
int x;

//Outras definicoes:
LiquidCrystal lcd(28,30,32,34,36,38);

bool flag = false, flag2 = false;
bool erroCom = false;
unsigned long tempo;

//Delay sem ser delay:
void delay2(int milsec){
  Serial.print("Tempo esperando: ");
  Serial.println(milsec);
  unsigned int tempo = millis();
  while((millis() - tempo) < milsec){  }
}

void setup() {
  //I2C:
  Wire.setTimeout(50);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  //Serial:
  Serial.begin(9600);

  //Inicializando variaveis:
  for(int i=0; i<QTD_ESTADOS;i++){
    fimEstado[i] = true;
  }
}

void loop() {
  //Parar motores caso parar de receber comandos.
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
  
  // AÇÕES ESTADOS:
  switch(estadoAtual){ 
    case 1: // ### Ações Segue Parede
      robo.follow_wall_to_terrine_area();
      
      fimEstado[1] = true;
      estadoAtual = 90;
    break;

    case 2: // ### Ações Procura Copo
      robo.adjust_to_get_cup();
      
      fimEstado[2] = true;
      estadoAtual = 90;
    break;

    case 3: // ### Ações Pega Copo
      robo.getTerrine();
      
      fimEstado[3] = true;
      estadoAtual = 90;
    break;

    case 4: // ### Ações para Chegar na Vaca:
      switch(subEstado){
        case 1: // Girar p/ encontrar vaca:
          robo.moveTank(-100,100);
        break;

        case 2: // Manobrar p/ o Lado:
          if(ladoV == 1){
            x =  1;
          }else if(ladoV = 2){
            x = -1;
          } 
          //Girar complementar do angulo recebido
          robo.moveTank((-VEL_ROB)*x,VEL_ROB*x);
          delay(TIME_90 - anguloV*ADJ_DEG);
          robo.moveTank(0,0);

          //Andar para frente porporcionalmente:
          robo.moveTank(VEL_ROB,VEL_ROB);
          delay(anguloV*ADJ_FOR);
          robo.moveTank(0,0);

          //Girar 90 graus
          robo.moveTank(VEL_ROB*x,(-VEL_ROB)*x);
          delay(TIME_90 - anguloV*ADJ_DEG);
          robo.moveTank(0,0);
          subEstado = 90;
        break;

        case 3: // Seguir em Frente Até a Vaca:
          // Serial.println("Indo em frente..");
          // delay(2000);
          // Serial.println("Parei de ir pra frente");
          subEstado = 90;
          estadoAtual = 90;
        break;

        case 90: // Sub-estado de Esperar Pelo Proximo Comando:
          robo.moveTank(0,0);
          flag = false;flag2 = false;
        break;
      }
    break;

    case 90: // ### Estado de espera por comandos:
      
    break;

    default:
      Serial.println("Error 404 - Not Found");
    break;
  }
}

// ### Função executada quando recebe comando do arduino:
void receiveData(int byteCount) {

  if(byteCount != QTD_BYTES_I2C){
    //Erro de comunicacao:
    Serial.println("Erro!, Msg descartada!");
    while(Wire.available()) {
      Wire.read();
    }
    clearBuf();
    erroCom = true;
  }else{
    //Sucesso comunicacao:
    clearBuf();
    erroCom = false;

    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
    }

    //Interpretando comandos vindos do rasp:
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

      //  ####  Procura/Segue vaca  #### 
      case 4:
        out[0]=4;
        switch(in[1]){
          case 1:// Inicio estado 4
            out[1] = 1;
            estadoAtual = 4;
          break;

          case 2:// Solicitando para girar/parar de procurar a vaca:
            estadoAtual = 4; 
            out[1] = 2;
            if(in[3] == 1){
              subEstado = 1;
              out[3] = 1;
            }else if(in[3] == 2){
              subEstado = 90;
              out[3] = 2;
              flag = true;flag2 = true;
            }else{
              out[0] = 98;
            }
          break;

          case 3:// Solicitando para fazer a manobra com os parametros:
            estadoAtual = 4;
            out[1] = 3;
            if(in[3] == 1 || in[3] == 2){
              ladoV = in[3];
              anguloV = in[4];
              subEstado = 2;
            }else{
              out[0] = 98;
            }
          break;

          case 4:// Perguntando se ja terminou o item acima:
            estadoAtual = 4;
            out[1] = 4;
            if(subEstado == 90){
              out[3] = 1;
            }else{
              out[3] = 2;
            }
          break;

          case 5:// Solicitando para ir para vaca
            estadoAtual = 4;
            out[1] = 5;
            subEstado = 3;
          break;

          case 6:// Perguntando se ja terminou o item acima:
            estadoAtual = 4;
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

      default:
        out[0] = 98;
      break;
    }
  }
}

//Função para enviar resposta para o rasp:
void sendData() {
  out[9] = ';';
  if(erroCom){
    Wire.write(msgPadrao, QTD_BYTES_I2C);
  }else{
    Wire.write(out, QTD_BYTES_I2C);
  }
}
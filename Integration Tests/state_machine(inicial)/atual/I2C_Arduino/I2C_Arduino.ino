#include <Wire.h>
#include "GIMu.h"
#include <LiquidCrystal.h>

// ### Definições Robô
Motor direito(DC11, DC12);
Motor esquerdo(DC21, DC22);
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
#define QTD_ESTADOS 9
#define BT_INICIO 13
int estadoAtual = 20;
int subEstado = 90;
bool fimEstado[QTD_ESTADOS];
bool comecarTudo = false;

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
int velE, velD;

//Definicoes arucos:
bool obstaculoEncontrado = false;

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
//Imprimit LCD:
void printLCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("E: ");
  lcd.print(estadoAtual);
  
  lcd.setCursor(0,10);
  lcd.print("E: ");
  lcd.print(estadoAtual);
}

void setup() {
  //I2C:
  Wire.setTimeout(50);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  //Imprimir:
  Serial.begin(9600);
  lcd.begin(16,2);
  
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
      estadoAtual = 20;
      flag2 = false;
    }
  }

  printLCD();

  // AÇÕES ESTADOS:
  switch(estadoAtual){ 
    case 1: // ### Ações Segue Parede
      robo.follow_wall_to_terrine_area();
      robo.moveFrente(0);
      
      fimEstado[1] = true;
      estadoAtual = 90;
    break;

    case 2: // ### Ações Procura Copo
      robo.adjust_to_get_cup();
      robo.moveFrente(0);
      
      fimEstado[2] = true;
      estadoAtual = 90;
    break;

    case 3: // ### Ações Pega Copo
      robo.getTerrine();
      robo.moveFrente(0);
      
      fimEstado[3] = true;
      estadoAtual = 90;
    break;

    case 4: // ### Ações para Chegar na Vaca:
      switch(subEstado){
        case 1: // Girar p/ encontrar vaca:
          robo.moveTank(-200,200);
          subEstado = 91;
        break;

        case 2:
          robo.moveTank(velE,velD);
        break;

        case 3: // Manobrar p/ o Lado:
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

        case 4: // Seguir em Frente Até a Vaca:
          robo.moveFrente(LOOKING_SPEED);
          while(1){
            if(robo.getSharp(SH_ORDENHADOR) <= TEM_TETA) break;
          }
          robo.moveFrente(0);
          subEstado = 90;
          estadoAtual = 90;
        break;

        case 90: // Sub-estado de Esperar Pelo Proximo Comando:
          robo.moveTank(0,0);
          flag = false;flag2 = false;
        break;
        default:

        break;
      }
    break;

    case 5: // ### Ações Ordenhar
      robo.ordenhar04();
      robo.moveFrente(0);

      fimEstado[5] = true;
      estadoAtual = 90;
    break;

    case 6: // ### Ações Ordenhar
      robo.milkTeta();
      robo.moveFrente(0);

      fimEstado[6] = true;
      estadoAtual = 90;
    break;

    case 7: // ### Ações para Chegar no tanque:
      switch(subEstado){
        case 1: // Girar p/ encontrar tag:
          robo.moveTank(-100,100);
        break;

        case 2: // Movimentar controlando:
          robo.moveTank(velE,velD);
        break;

        case 3:
          //CODIGO PARA DESVIAR DA PORRA DA MERDA DA PAREDE
          robo.follow_wall_to_little_gate();
          robo.moveFrente(0);
          subEstado = 90;
          estadoAtual = 90;
        break;

        case 90: // Sub-estado de Esperar Pelo Proximo Comando:
          robo.moveFrente(0);
          flag = false;flag2 = false;
        break;
      }
    break;

    case 8: // ### Ações para derramar :
      robo.adjust_to_derramar_leite();
      robo.dropMilk();
      robo.moveFrente(0);

      fimEstado[8] = true;
      estadoAtual = 20;
      comecarTudo = false;
    break;

    case 20:
      if(analogRead(BT_INICIO) > 900){
        comecarTudo = true;
      }
    break;

    case 90: // ### Estado de espera por comandos:
      //haHAA
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
            out[1] = 1;  
            estadoAtual = 1;
            fimEstado[1] = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimEstado[1]){
              out[2] = 1;
            }else{
              out[2] = 2;
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
            out[1] = 1;  
            estadoAtual = 2;
            fimEstado[2] = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimEstado[2]){
              out[2] = 1;
            }else{
              out[2] = 2;
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
            out[1] = 1;  
            estadoAtual = 3;
            fimEstado[3] = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimEstado[3]){
              out[2] = 1;
            }else{
              out[2] = 2;
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
            out[1] = 2;
            estadoAtual = 4; 
            if(in[2] == 1){
              subEstado = 1;
              flag = true;flag2 = true;
            }else if(in[2] == 2){
              subEstado = 90;
            }else{
              out[0] = 98;
            }
          break;

          case 3:
            out[1] = 3;  
            estadoAtual = 4;
            velE = (in[2]-125)*2;
            velE = (in[3]-125)*2;
            subEstado = 2;
            flag = true;flag2 = true;
          break;

          case 4:// Solicitando para fazer a manobra com os parametros:
            out[1] = 4;  
            estadoAtual = 4;
            if(in[2] == 1 || in[2] == 2){
              ladoV = in[2];
              anguloV = in[3];
              subEstado = 3;
            }else{
              out[0] = 98;
            }
          break;

          case 5:// Perguntando se ja terminou o item acima:
            out[1] = 5;  
            estadoAtual = 4;
            if(subEstado == 90){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;

          case 6:// Solicitando para ir para vaca
            out[1] = 6;
            estadoAtual = 4;
            subEstado = 4;
          break;

          case 7:// Perguntando se ja terminou o item acima:
            out[1] = 6;  
            estadoAtual = 4;
            if(subEstado == 90){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;

          default:
            out[0] = 98;
          break;
        }
      break;

      //  ####  Ordenhar  #### 
      case 5:
        out[0]=5;
        switch(in[1]){
          case 1://Mandando arduino começar.
            out[1] = 1;  
            estadoAtual = 5;  
            fimEstado[5] = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimEstado[5]){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;
          
          default:
            out[0] = 98;
          break;
        }
      break;

      //  ####  Chupa Chupa  #### 
      case 6:
        out[0]=6;
        switch(in[1]){
          case 1://Mandando arduino começar.
            out[1] = 1;  
            estadoAtual = 6;
            fimEstado[6] = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimEstado[6]){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;
          
          default:
            out[0] = 98;
          break;
        }
      break;
      
      //  ####  Segue Aruco até infinito   #### 
      case 7:
        out[0]=7;
        switch(in[1]){
          case 1:// Inicio estado 7
            out[1] = 1;
            estadoAtual = 7;
          break;

          case 2:// Solicitando para girar/parar de procurar a tag:
            out[1] = 2;
            estadoAtual = 7; 
            if(in[2] == 1){
              subEstado = 1;
              out[2] = 1;
              flag = true;flag2 = true;
            }else if(in[2] == 2){
              subEstado = 90;
              out[2] = 2;
              flag = false;flag2 = false;
            }else{
              out[0] = 98;
            }
          break;

          case 3:
            out[1] = 3;  
            estadoAtual = 7;
            if(obstaculoEncontrado){
              velE = 0;
              velD = 0;
              out[2] = 1;
            }else{
              velE = (in[2]-125)*2;
              velE = (in[3]-125)*2;
              out[2] = 2;
            }
            subEstado = 2;
            flag = true;flag2 = true;
          break;

          case 4:// Solicitando para ir para vaca
            out[1] = 4;  
            estadoAtual = 7;
            subEstado = 3;
            flag = false;flag2 = false;
          break;

          case 5:// Perguntando se ja terminou o item acima:
            out[1] = 5;
            estadoAtual = 7;
            if(subEstado == 90){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;

          default:
            out[0] = 98;
          break;
        }
      break;

      case 8:
        out[0]=8;
        switch(in[1]){
          case 1://Mandando arduino começar.
            estadoAtual = 8;
            out[1] = 1;
            fimEstado[8] = false;
          break;

          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(fimEstado[8]){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;
          
          default:
            out[0] = 98;
          break;
        }
      break;

      case 20:
        out[0] = 20;
        switch(in[1]){
          case 1:
            out[1] = 1;
            if(comecarTudo){
              out[2] = 1;
            }else{
              out[2] = 2;
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
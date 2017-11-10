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
int estadoAtual = 1;
int subEstado = 1;
bool comecarTudo = false;
byte ladoCopo = 0;

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

  // printLCD();

  // AÇÕES ESTADOS:
  switch(estadoAtual){ 

    case 1: 
      switch(subEstado){
        case 1:
          if(digitalRead(PIN_START) == 1){
            subEstado = 0;
            comecarTudo = true;
            blocked = false;
          }
        break;
        default:
          cout << "Erro substado, Estado: 1" << endl;
        break;
      }
    break;

    case 2:
      switch(subEstado){
        case 1:
          if(blocked){
            robo.moveFrente(0);
            flag = false;flag2 = false;
          }else{
            robo.moveTank(velE,velD);
            if(robo.getSharp(SH_FRENTE_DIREITA) < 20 || robo.getSharp(SH_FRENTE_ESQUERDA) < 20){
              blocked = true;
            }
          }
        break;
        case 2:
          robo.follow_wall_to_the_terrine2();
          robo.moveFrente(0);
          subEstado = 0;
        break;
        default:
          cout << "Erro substado, Estado: 1" << endl;
        break;
      }
    break;

    case 3:
      switch(subEstado){
        case 1:
          //A FUNCAO DE ESTACIONAR NA REGIÃO DO COPO VAI AQUI:

          //
          subEstado = 0;
          estadoAtual = 0;

        break;
      }
    break;

    case 4: // ### Ações para Chegar na Vaca:
      switch(subEstado){
        case 1: // Girar p/ encontrar vaca:
          robo.moveTank(-150,150);
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

        break;
      }
    break;

    case 5:
      switch(subEstado){
        case 1:
          //A FUNCAO DE ORDENHAR A VACA VAI AQUI:
          
          //

          blocked = false;
          subEstado = 0;
          estadoAtual = 0;
        break;
      }
    break;

    case 6:
      switch(subEstado){
        case 1:
          if(blocked){
            robo.moveFrente(0);
            flag = false;flag2 = false;
          }else{
            robo.moveTank(velE,velD);
            if(robo.getSharp(SH_FRENTE_DIREITA) < 20 || robo.getSharp(SH_FRENTE_ESQUERDA) < 20){
              blocked = true;
            }
          }
        break;
        case 2:
          //FUNCAO PARA DESVIAR DA PORTEIRA VAI AQUI:

          //

          blocked = false;
        break;
      }
    break;

    case 7:
      switch(subEstado){
        case 1:
          //A FUNCAO DE ORDENHAR O COPO VAI AQUI:
          
          //

          blocked = false;
          subEstado = 0;
          estadoAtual = 0;
        break;
      }
    break;

    default:
      Serial.println("Error 404 - Not Found");
    break;
  }
}

// #################################### Função executada quando recebe comando do arduino:
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
      //  ####  START  #### 
      case 1:
        out[0]=1;
        estadoAtual = 1;
        switch(in[1]){
          case 1:
            out[1] = 1;
            if(comecarTudo){
              out[2] = 1;
              subEstado = 0;
            }else{
              out[2] = 2;
            }
          break;
          case 2:
            out[1] = 2;
            subEstado = 2;
          break;
          default:
            out[1] = 98;
          break;
        }
      break;
      
      //  ####  VAI PROS COPOS  #### 
      case 2:
        out[0]=2;
        estadoAtual = 2;
        switch(in[1]){
          case 1:
            out[1]=1;
            subEstado = 1;
            velE = (in[2]-125)*2;
            velD = (in[3]-125)*2;
            flag = true;flag2 = true;
            if(blocked){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;
          case 2:
            out[1]=2;
            ladoCopo = in[2];
            subEstado = 2;
          break;
          case 3:
            out[1]=3;
            if(subEstado!=2){
              out[2]=1;
            }else{
              out[2]=2;
            }
          break;
          default:
            out[0] = 98;
          break;
        }
      break;

      //  ####  PEGAR COPO  #### 
      case 3:
        out[0]=3;
        estadoAtual = 3;
        switch(in[1]){
          case 1://Mandando arduino começar.
            out[1] = 1;  
            subEstado = 1;
          break;
          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(subEstado!=1){
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

      //  ####  GO TO VACA  #### 
      case 4:
        out[0]=4;
        estadoAtual = 4;
        switch(in[1]){
          case 1:
            out[1] = 1;
            velE = (in[2]-125)*2;
            velD = (in[3]-125)*2;
            subEstado = 2;
            flag = true;flag2 = true;
          break;

          case 2:// Solicitando para fazer a manobra com os parametros:
            out[1] = 2;
            if(in[2] == 1 || in[2] == 2){
              ladoV = in[2];
              subEstado = 3;
            }else{
              out[0] = 98;
            }
          break;

          case 3:// Perguntando se ja terminou o item acima:
            out[1] = 3;
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

      //  ####  ORDENHAR  #### 
      case 5:
        out[0]=5;
        estadoAtual = 5;
        switch(in[1]){
          case 1://Mandando arduino começar.
            out[1] = 1;  
            subEstado = 1;
          break;
          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(subEstado!=1){
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

      //  ####  IR PARA TANQUE  #### 
      case 6:
        out[0]=6;
        estadoAtual = 6;
        switch(in[1]){
          case 1:
            out[1]=1;
            subEstado = 1;
            velE = (in[2]-125)*2;
            velD = (in[3]-125)*2;
            flag = true;flag2 = true;
            if(blocked){
              out[2] = 1;
            }else{
              out[2] = 2;
            }
          break;
          case 2://Mandando arduino começar.
            out[1] = 2;  
            subEstado = 2;
          break;
          case 3://Perguntando se ja terminou.
            out[1] = 2;
            if(subEstado!=2){
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
      
      //  ####  DERRAMAR NO TANQUE  #### 
      case 7:
        out[0]=7;
        estadoAtual = 7;
        switch(in[1]){
          case 1://Mandando arduino começar.
            out[1] = 1;  
            subEstado = 1;
          break;
          case 2://Perguntando se ja terminou.
            out[1] = 2;
            if(subEstado!=1){
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
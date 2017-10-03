#include "libi2c/pi2c.cpp"
#include  "extras.h"
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

using namespace std;

#define BYTES 10

int main(){
Extras extras;
string data  =  "";
string csvHeader = "motor1,motor2,sensor1,sensor2,sensor3,sensor4,sensor5,sensor6";
string filename = "teste.csv";
    	Pi2c arduino(4);
   	int qtdErro = 0;
    
	char buf[BYTES*4];
	for(int i=0;i<BYTES*4;i++){
		buf[i] = '\0';
	}
	
    char cmdF[BYTES+1] = {"F........;"};
	char cmdT[BYTES+1] = {"T........;"};
	char cmdD[BYTES+1] = {"D........;"};
	char cmdE[BYTES+1] = {"E........;"};
	char cmdI[BYTES+1] = {"I........;"};
	char cmdS[BYTES+1];
	
     

	while(1){
		char input = getchar();
		switch(input){
			case 'w':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdF[i];
				}
				data+= "150,150";
			break;
			case 's':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdT[i];
				}
				data+= "-150,-150";
			break;
			case 'd':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdD[i];
				}
				data+= "150,-150";
			break;
			case 'a':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdE[i];
				}
				data+= "-150,150";
			break;
			case 'i':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdI[i];
				}
			break;
			default:
				return 0;
			break;
		}
		cin.ignore();
		
		//Comando para Andar:
		arduino.i2cWrite(cmdS, BYTES);
		usleep(10000);

		if(arduino.i2cRead(buf,BYTES) == BYTES){
			buf[(BYTES*4)-1] = '\0';
		}else{		
			cout << "Erro : " << endl;
		}
		for(int i=0;i<BYTES*4;i++){
			buf[i] = '\0';
		}

		//Receber Dados:
		usleep(1100000);
		for(int i=0;i<BYTES+1;i++){
			cmdS[i]=cmdI[i];
		}
		arduino.i2cWrite(cmdS, BYTES);
		usleep(10000);

		int aux;
		if(arduino.i2cRead(buf,BYTES) == BYTES){
			buf[(BYTES*4)-1] = '\0';
			for(int i=0;i<6;i++){
				cout << " " << i << ": " << (int)buf[i];
				aux = (int)buf[i];
				data+=  "," + to_string(aux);
			}
			/*
			0 e 1 -> Frente
			2 e 3 -> Esquerda
			4 e 5 -> Direita
			*/
			cout << endl;

			extras.logCsv(data.c_str(),filename.c_str() ,csvHeader.c_str());
		}else{		
			cout << "Erro : " << endl;
		}
		for(int i=0;i<BYTES*4;i++){
			buf[i] = '\0';
		}

	}
    return 0;
}

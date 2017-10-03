#include "libi2c/pi2c.cpp"
#include <iostream>
#include <string>
#include <fstream>  
#include <stdio.h>
#include <sys/stat.h>

using namespace std;

#define BYTES 10

int main(){
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
				cmdS[1] = (char) (-190);
				cmdS[2] = (char) (180);
			break;
			case 's':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdT[i];
				}
			break;
			case 'd':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdD[i];
				}
			break;
			case 'a':
				for(int i=0;i<BYTES+1;i++){
					cmdS[i]=cmdE[i];
				}
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
			}
			/*
			0 e 1 -> Frente
			2 e 3 -> Esquerda
			4 e 5 -> Direita
			*/
			cout << endl;

		}else{		
			cout << "Erro : " << endl;
		}
		for(int i=0;i<BYTES*4;i++){
			buf[i] = '\0';
		}

		

	}
    return 0;
}

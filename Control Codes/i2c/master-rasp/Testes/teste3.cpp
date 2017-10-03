#include "libi2c/pi2c.cpp"
#include <iostream>
#include <string>
#include <fstream>  
#include <stdio.h>
#include <sys/stat.h>

using namespace std;

#define BYTES 10

Pi2c arduino(4);

int qtdErro = 0;
char buf[BYTES*4];
char cmdS[BYTES+1];

void clear(){
	for(int i=0;i<BYTES*4;i++){
		buf[i] = '\0';
	}
}

void getSharp(){
	char cmdI[BYTES+1] = {"I........;"};
	//Receber Dados:

	arduino.i2cWrite(cmdI, BYTES);
	usleep(10000);

	if(arduino.i2cRead(buf,BYTES) == BYTES){
		/*
		0 e 1 -> Frente
		2 e 3 -> Esquerda
		4 e 5 -> Direita
		*/
		cout << endl;
	}else{		
		cout << "Erro : " << endl;
	}
	clear();
}

void enviarDados(){
	//Comando para Andar:
	arduino.i2cWrite(cmdS, BYTES);
	usleep(10000);

	if(arduino.i2cRead(buf,BYTES) == BYTES){
		buf[(BYTES*4)-1] = '\0';
	}else{		
		cout << "Erro : " << endl;
	}
	clear();
}




int main(){
	while(1){
		getSharp();
		cout << buf[0] << endl;

		int val = 110;
		cmdS[0] = (char) val;
		enviarDados();

		usleep(1000000);
	}
    return 0;
}

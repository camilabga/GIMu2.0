#include "I2C.h"

I2C::I2C(){
    clearBuf();
}

void I2C::clearBuf(){
	for(int i=0;i<BYTES*4;i++){
		buf[i] = '\0';
	}
}

void I2C::getData(){
    Pi2c* ard = new Pi2c(4);
	char cmdI[BYTES+1] = {"I........;"};
	
    //Receber Dados:
	clearBuf();
	ard->i2cWrite(cmdI, BYTES);
	usleep(10000);

	if(ard->i2cRead(buf,BYTES) == BYTES){
		/*
		0 e 1 -> Frente
		2 e 3 -> Esquerda
		4 e 5 -> Direita
		*/
		cout << endl;
	}else{		
		cout << "Erro : " << endl;
	}
	
    ard->~Pi2c();
}

void I2C::enviarDados(){
    Pi2c* ard = new Pi2c(4);

	//Comando para Andar:
	ard->i2cWrite(cmdS, BYTES);
	usleep(10000);

	if(ard->i2cRead(buf,BYTES) == BYTES){
		buf[(BYTES*4)-1] = '\0';
	}else{		
		cout << "Erro : " << endl;
	}
	clearBuf();
    ard->~Pi2c();
}

#include "I2C.h"

I2C::I2C(){
    clearBuf();
}

void I2C::clearBuf(){
	for(int i=0;i<QTD_BYTES_I2C*4;i++){
		buf[i] = '\0';
	}
}

void I2C::getData(){
    Pi2c* ard = new Pi2c(4);
	char cmdI[QTD_BYTES_I2C+1] = {"I........;"};
	
    //Receber Dados:
	clearBuf();
	ard->i2cWrite(cmdI, QTD_BYTES_I2C);
	usleep(10000);

	if(ard->i2cRead(buf,QTD_BYTES_I2C) == QTD_BYTES_I2C){
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
	ard->i2cWrite(cmdS, QTD_BYTES_I2C);
	usleep(10000);

	if(ard->i2cRead(buf,QTD_BYTES_I2C) == QTD_BYTES_I2C){
		buf[(QTD_BYTES_I2C*4)-1] = '\0';
	}else{		
		cout << "Erro : " << endl;
	}
	clearBuf();
    ard->~Pi2c();
}

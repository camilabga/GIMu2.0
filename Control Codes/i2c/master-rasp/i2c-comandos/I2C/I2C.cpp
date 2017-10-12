#include "I2C.h"

I2C::I2C(){
    clearBuf();
}

void I2C::clearBuf(){
	for(int i=0;i<QTD_BYTES_I2C*4;i++){
		in[i] = '\0';
	}
}

bool I2C::getData(){
	Pi2c* ard = new Pi2c(4);
	clearBuf();

	//Enviar comando para receber resposta:
	out[0] = 'I';
	out[9] = ';';
	ard->i2cWrite(out, QTD_BYTES_I2C);
	usleep(10000);

	//Recebe e checa se deu certo:
	if(ard->i2cRead(in,QTD_BYTES_I2C) == QTD_BYTES_I2C && in[9] == ';'){
		ard->~Pi2c();
		return true;
	}else{
		ard->~Pi2c();
		return false;
	}
}

bool I2C::sendData(){
    Pi2c* ard = new Pi2c(4);
	char inAux[QTD_BYTES_I2C*4];

	//Enviar dados para arduino:
	ard->i2cWrite(out, QTD_BYTES_I2C);
	usleep(10000);

	//Recebendo confirmação:
	if(ard->i2cRead(inAux,QTD_BYTES_I2C) == QTD_BYTES_I2C && inAux[9] == ';'){
		ard->~Pi2c();
		return true;
	}else{
		ard->~Pi2c();
		return false;
	}
}

bool I2C::tradeData(int milisec){
	Pi2c* ard = new Pi2c(4);
	clearBuf();

	//Enviar dados para arduino:
	ard->i2cWrite(out, QTD_BYTES_I2C);
	usleep(milisec * 1000);

	//Recebendo dados do arduino:
	if(ard->i2cRead(in,QTD_BYTES_I2C) == QTD_BYTES_I2C && in[9] == ';'){
		ard->~Pi2c();
		return true;
	}else{
		ard->~Pi2c();
		return false;
	}
}

void I2C::printData(){
	cout << "Dados recebidos-> " ;
	for(int i=0;i<QTD_BYTES_I2C;i++){
		cout << "B" << i << ": " << (int) in[i] << " " ;
	}
	cout << endl;
}
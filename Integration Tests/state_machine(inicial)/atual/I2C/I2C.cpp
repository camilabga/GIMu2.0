#include "I2C.h"
//
I2C::I2C(){
    clearBufIn();
	clearBufOut();
}

void I2C::clearBufIn(){
	for(int i=0;i<QTD_BYTES_I2C*4;i++){
		in[i] = '\0';
	}
}
void I2C::clearBufOut(){
	for(int i=0;i<QTD_BYTES_I2C;i++){
		out[i]= '\0';
	}
	out[9] = ';';
}

bool I2C::getData(){
	Pi2c* ard = new Pi2c(ADD_I2C);
	clearBufIn();

	//Enviar comando para receber resposta:
	out[0] = 'I';
	out[9] = ';';
	ard->i2cWrite(out, QTD_BYTES_I2C);
	usleep(20000);

	//Recebe e checa se deu certo:
	if(ard->i2cRead(in,QTD_BYTES_I2C) == QTD_BYTES_I2C && in[9] == ';'){
		ard->~Pi2c();
		clearBufOut();
		return true;
	}else{
		ard->~Pi2c();
		clearBufOut();
		return false;
	}
}

bool I2C::sendData(){
    Pi2c* ard = new Pi2c(ADD_I2C);
	char inAux[QTD_BYTES_I2C*4];

	//Enviar dados para arduino:
	ard->i2cWrite(out, QTD_BYTES_I2C);
	usleep(20000);

	//Recebendo confirmação:
	if(ard->i2cRead(inAux,QTD_BYTES_I2C) == QTD_BYTES_I2C && inAux[9] == ';'){
		ard->~Pi2c();
		clearBufOut();
		return true;
	}else{
		ard->~Pi2c();
		clearBufOut();
		return false;
	}
}

bool I2C::tradeData(int milisec){
	Pi2c* ard = new Pi2c(ADD_I2C);
	clearBufIn();

	//Enviar dados para arduino:
	ard->i2cWrite(out, QTD_BYTES_I2C);
	usleep(milisec * 1000);

	//Recebendo dados do arduino:
	if(ard->i2cRead(in,QTD_BYTES_I2C) == QTD_BYTES_I2C && in[9] == ';'){
		ard->~Pi2c();
		clearBufOut();
		return true;
	}else{
		ard->~Pi2c();
		clearBufOut();
		return false;
	}
}

bool I2C::sendFunc(unsigned char b0, unsigned char b1, int b2 = 0, int b3 = 0, int b4 = 0){
	switch(b0){
		case 1:
			out[0] = 1;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 2:
			out[0] = 2;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 3:
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 4:
			switch(b1){
				case 1:
					out[1] = 1;
				break;
				case 2:
					out[2] = 2;
					out[3] = (unsigned char) b2;
					out[4] = (unsigned char) b3;
				break;
				default:
					cout << "Erro: func not defined" << endl;
					return false;
				break;
			}
		break;
		default:
			cout << "Erro: func not defined" << endl;
			return false;
		break;
	}	

	tradeData();
	usleep(100000);
}

void I2C::printData(){
	cout << "I2C-> " ;
	for(int i=0;i<QTD_BYTES_I2C;i++){
		cout << "B" << i << ": " << (int) in[i] << " " ;
	}
	cout << endl;
}
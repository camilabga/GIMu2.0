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
	usleep(10000);

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
	unsigned char inAux[QTD_BYTES_I2C*4];

	//Enviar dados para arduino:
	ard->i2cWrite(out, QTD_BYTES_I2C);
	usleep(10000);

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
	bool tryAgain = true;

	//Enviar dados para arduino:
	while(tryAgain){
		ard->i2cWrite(out, QTD_BYTES_I2C);
		usleep(milisec * 1000);

		//Recebendo dados do arduino:
		if(ard->i2cRead(in,QTD_BYTES_I2C) == QTD_BYTES_I2C && in[9] == ';'/* && in[0] == out[0] && in[1] == out[1]*/){
			tryAgain = false;
		}else{
			cout << "Erro I2C: error on send msg, trying again..." << endl;
			tryAgain = true;
			usleep(200000);
		}
	}
	ard->~Pi2c();
	return true;
}

bool I2C::sendFunc(unsigned char b0, unsigned char b1, int b2, int b3){
	switch(b0){
		case 1: //SEGUE PAREDE
			out[0] = 1;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 2: //ACHA COPO
			out[0] = 2;
			switch(b1){
				case 1:
					out[1] = 1;	
					out[2] = (unsigned char) b2;
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 3: //PEGA COPO
			out[0] = 3;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 4: //VACA
			out[0] = 4;
			switch(b1){
				case 1:
					out[1] = 1;
				break;
				case 2:
					if(!(b2 == 1 || b2 == 2)){
						cout << "Erro I2C: parameter not defined asduhasudh" << endl;
						return false;
					} 
					out[1] = 2;
					out[2] = (unsigned char) (b2);
				break;
				case 3:
					out[1]=3;
					out[2] = (unsigned char) ((b2/2)+125);
					out[3] = (unsigned char) ((b3/2)+125);
				break;
				case 4:
					if(!(b2 == 1 || b2 == 2)){
						cout << "Erro I2C: parameter not defined" << endl;
						return false;
					}
					out[1] = 4;
					out[2] = (unsigned char) b2;
					out[3] = (unsigned char) b3;
				break;
				case 5:
					out[1] = 5;
				break;
				case 6:
					out[1] = 6;
				break;
				case 7:
					out[1] = 7;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 5: //ENCAIXA TETA
			out[0] = 5;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 6: //CHUPA CHUPA
			out[0] = 6;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 7: //ARUCO
			out[0] = 7;
			switch(b1){
				case 1:
					out[1] = 1;
				break;
				case 2:
					if(!(b2 == 1 || b2 == 2)){
						cout << "Erro I2C: parameter not defined" << endl;
						return false;
					} 
					out[1] = 2;
					out[2] = (unsigned char) (b2);
				break;
				case 3:
					out[1]=3;
					out[2] = (unsigned char) ((b2/2)+125);
					out[3] = (unsigned char) ((b3/2)+125);
				break;
				case 4:
					out[1] = 4;
				break;
				case 5:
					out[1] = 5;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 8: //DERRAMA O LEITE
			out[0] = 8;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				case 2:
					out[1] = 2;
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		case 20:
			out[0] = 20;
			switch(b1){
				case 1:
					out[1] = 1;	
				break;
				default:
					cout << "Erro I2C: func not defined" << endl;
					return false;
				break;
			}
		break;
		default:
			cout << "Erro I2C: state not defined" << endl;
			return false;
		break;
	}	
	out[9] = ';';
	tradeData();
	usleep(10000);
	return true;
}

void I2C::printData(){
	cout << "I2C Data:  " ;
	for(int i=0;i<QTD_BYTES_I2C;i++){
		cout << "B" << i << ": " << (int) in[i] << " " ;
	}
	cout << endl;
}
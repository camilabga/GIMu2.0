#include "libi2c/pi2c.cpp"
#include <iostream>

using namespace std;

#define BYTES 10

void clearBuf(char buf[]){
	for(int i=0;i<BYTES*4;i++){
		buf[i] = '\0';
	}	
}

int main(){
	//Endereço 0x04 I2C:
    	Pi2c arduino(4);
    
	char buf[BYTES*4];
	clearBuf(buf);
    	char cmd[BYTES+1] = {"012345678;"};
     	
	//
	while(1){		
		//Enviar comando:
		arduino.i2cWrite(cmd, BYTES);
		usleep(10000);
		
		//Receber respostas:
		if(arduino.i2cRead(buf,BYTES) == BYTES){
			buf[(BYTES*4)-1] = '\0';
			cout << "Retorno: " << buf << endl;
		}else{		
			cout << "Erro!" << endl;
		}
		clearBuf(buf);
	}
	return 0;
}

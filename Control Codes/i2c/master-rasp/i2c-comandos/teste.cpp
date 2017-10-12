#include "I2C/I2C.h"

int main(){
    I2C arduino;
	while(1){
		arduino.getData(); //Solicita ao arduino informações

		//Loop para imprimir os valores do buf de entrada "in";
		for(int i=0;i<QTD_BYTES_I2C;i++){
			cout << (int)arduino.in[i] << " "; //Acessando os valores do buf de entrada; 
		}
		cout << endl;
		//
		int val = 110;
		arduino.out[0] = (char) val; //Preencher primeira posição do buf de bytes (char) de saida
		arduino.sendData(); //Envia os valores contidos no buf de saida "out" para o Arduino

		usleep(1000000); // Nao ha nescessidade, apenas para testes visuais.
	}
    return 0;
}
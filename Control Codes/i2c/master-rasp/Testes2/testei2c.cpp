#include </home/pi/Documents/GIMu2.0/Control Codes/i2c/master-rasp/Testes2/I2C/I2C.h>
int main(){
    I2C arduino;
	while(1){
		if(!arduino.getData())  cout << "Erro!" << endl; //Solicita ao arduino informações
		arduino.printData();
		

		int val = -75;
		int aux = 75;
		arduino.out[0] = 'M';
		arduino.out[1] = (char) val;
		arduino.out[2] = (char) aux; //Preencher primeira posição do buf de bytes (char) de saida
		if(!arduino.sendData()) cout << "Erro!" << endl; //Envia os valores contidos no buf de saida "out" para o Arduino
		usleep(100000);

		// arduino.out[0] = 'S';
		// arduino.sendData();
	// 
		// usleep(10000); // Nao ha nescessidade, apenas para testes visuais.
	}
    return 0;
}
//
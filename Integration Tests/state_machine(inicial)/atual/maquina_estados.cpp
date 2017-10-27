#include <iostream>
#include "I2C/I2C.h"

using namespace std;

int main(){
	//Variaveis I2C:
	I2C arduino;
	//

	//Variaveis Máquina_Estado:
	int estadoAtual = 1, estadoAnterior;
	bool fim_geral = false;
	//

	while(!fim_geral){
		switch(estadoAtual){
			case 1:
				//Seguir parede:
				//Comando iniciar:
				arduino.out[0] = 1;
				arduino.out[1] = 1;
				arduino.tradeData();
				usleep(100000);
				while(1){
					arduino.out[0] = 1;
					arduino.out[1] = 2;
					arduino.tradeData();
					cout << (int)arduino.in[0] << " " << (int)arduino.in[1] << " " << (int)arduino.in[3] << endl;
					if(arduino.in[3] == 1){
						break;
					}
					cout << "Acabou Nao." << endl;
					usleep(100000);
				}
				cout << "Acabou Sim." << endl;
				fim_geral = true;
			break;
			case 2:
				//Procura Copo:

			break;

			default:
				//Erro:
			break;
		}
	}
    return 0;
}
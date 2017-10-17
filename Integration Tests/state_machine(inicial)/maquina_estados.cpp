#include "I2C/I2C.h"

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
				//Segue Parede:
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
#include <iostream>
#include "I2C/I2C.h"

using namespace std;

int main(){
	//Variaveis I2C:
	I2C arduino;
	//

	//Variaveis Máquina_Estado:
	int estadoAtual=1;
	bool fim_geral = false;
	//
//
	while(!fim_geral){
		switch(estadoAtual){
			case 1:
				//Segue Parede:
				//Teste:
				arduino.out[0] = 250;
				if(!arduino.tradeData()){
					cout << "Erro ao enviar! " << endl;
					usleep(1000000);
				}
				cout << "Enviado.. " << "Confirmacao: " << arduino.in << endl;
				usleep(100000);



				arduino.out[0] = 120;
				if(!arduino.tradeData()){
					cout << "Erro ao enviar! " << endl;
					usleep(1000000);
				}
				//Teste
				cout << "Enviado.. " << "Confirmacao: " << arduino.in << endl;
				//TEste
				usleep(100000);

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
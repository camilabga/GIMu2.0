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
				//Segue Parede:
				//Teste:
				arduino.out[0] = 'V';
				arduino.out[1] = 'D';
				arduino.out[2] = 'C';
				if(!arduino.tradeData()){
					cout << "Erro ao enviar! " << endl;
					return -1;
				}
				cout << "Enviado.. " << "Confirmacao: " << arduino.in << endl;
				usleep(100000);
				// if(!arduino.sendData()){
				// 	cout << "Erro ao enviar! " << endl;
				// 	return -1;
				// }
				// //
				// if(!arduino.getData()){
				// 	cout << "Erro ao receber! " << endl;
				// 	return -1;
				// }
				arduino.out[0] = 'D';
				arduino.out[1] = 'E';
				arduino.out[2] = 'U';
				if(!arduino.tradeData()){
					cout << "Erro ao enviar! " << endl;
					return -1;
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
//
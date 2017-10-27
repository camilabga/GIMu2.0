#include <iostream>
#include "I2C/I2C.h"
#include "Libs/Cow.h"

using namespace std;
using namespace cv;


int main(){
	//Variaveis I2C:
	I2C arduino;
	//

	//Variaveis Máquina_Estado:
	int estadoAtual = 4;
	bool fim_geral = false;
	//

	//Variaveis Opencv:
	VideoCapture capture(0);
	if ( !capture.isOpened() ){
	cout << "Cannot open the video file" << endl;
	return -1;
	}

	/*create Cow -> first initialization has no center
	and the still scans the whole Mat, also, do not contain
	any rectangle center defined */

	Cow cow;
	Mat frame;
	char c1;
	char c2;
	//

	while(!fim_geral){
		switch(estadoAtual){
			
			case 1:	//Seguir parede:
				
				//Envia comando I2C: Iniciando estado
				arduino.out[0] = 1;
				arduino.out[1] = 1;
				arduino.tradeData();
				usleep(100000);
				//

				while(1){

					//Envia comando I2C: Perguntando se já acabou
					arduino.out[0] = 1;
					arduino.out[1] = 2;
					arduino.tradeData();
					if(arduino.in[3] == 1){
						break;
					}
					//

					cout << "N Acabou 01." << endl;
					usleep(100000);
				}

				//Fim de estado:
				cout << "Acabou 01." << endl;
				estadoAtual = 2;
				//fim_geral = true;
				//
			break;
			
			case 2:	//Procura Copo:
				
				//Envia comando I2C: Iniciando estado
				arduino.out[0] = 2;
				arduino.out[1] = 1;
				arduino.tradeData();
				usleep(100000);
				//

				while(1){
					
					//Envia comando I2C: Perguntando se já acabou
					arduino.out[0] = 2;
					arduino.out[1] = 2;
					arduino.tradeData();
					if(arduino.in[3] == 1){
						break;
					}
					//

					cout << "N Acabou 02." << endl;
					usleep(100000);
				}

				//Fim de estado:
				cout << "Acabou 02." << endl;
				estadoAtual = 3;
				// fim_geral = true;
				//
			break;
			
			case 3:	//Pega Copo

				//Envia comando I2C: Iniciando estado
				arduino.out[0] = 3;
				arduino.out[1] = 1;
				arduino.tradeData();
				usleep(100000);
				//

				while(1){

					//Envia comando I2C: Perguntando se já acabou
					arduino.out[0] = 3;
					arduino.out[1] = 2;
					arduino.tradeData();
					if(arduino.in[3] == 1){
						break;
					}
					//

					cout << "N Acabou 03." << endl;
					usleep(100000);
				}

				//Fim de estado:
				cout << "Acabou 03." << endl;
				estadoAtual = 4;
				// fim_geral = true;
				//
			break;
				
			case 4:	//Procura vaca:
				
				//open webcam
				

				//Envia comando I2C: Iniciando estado
				arduino.out[0] = 4;
				arduino.out[1] = 1;
				arduino.tradeData();
				usleep(100000);
				//

				while(1){
					

					//PROCESSO OPENCV:
					if (!capture.read(frame)) {
						cout<<"\n Cannot read the video file. \n";
						break;
					}

					cow.setROI(frame);
					cow.transformImage(); 
					cow.searchSquares();

					if (cow.find()){
						cow.drawCenter(frame);
						cow.sendPID(c1,c2);
					} else {
						cow.sendPID(c1,c2);
					}

					namedWindow("Original", WINDOW_NORMAL);
					resizeWindow("Original", WIDTH, HEIGHT);
					imshow("Original", frame);

					if (waitKey(1) == 27){
						break;
					}
					//

					//Envia comando I2C: Movimentação
					arduino.out[0] = 4; //Estado 4
					arduino.out[1] = 2;	//Subfunção 2: envio de comando.
					arduino.out[3] = c1;//Comando 1
					arduino.out[4] = c2;//Comando 2
					arduino.tradeData();
					//

					cout << "N Acabou 04." << endl;
					usleep(10000);
				}

				//Fim de estado:
				cout << "Acabou 04." << endl;
				// estadoAtual = 5;
				fim_geral = true;
				//
			break;

			default:
				//Erro:
			break;
		}
	}
    return 0;
}
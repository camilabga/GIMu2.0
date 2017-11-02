#include <iostream>
#include "I2C/I2C.h"
#include "Libs/Cow.h"
//
using namespace std;
using namespace cv;


int main(){
	//Variaveis I2C:
	I2C arduino;
	//

	//Variaveis Máquina_Estado:
	int estadoAtual = 4;
	bool fim_geral = false;
	int teste;
	//

	//Variaveis Opencv:
	// VideoCapture capture(0);
	// if ( !capture.isOpened() ){
	// cout << "Cannot open the video file" << endl;
	// return -1;
	// }
	/*create Cow -> first initialization has no center
	and the still scans the whole Mat, also, do not contain
	any rectangle center defined */
	// Cow cow;
	// Mat frame;
	unsigned char c1;
	unsigned char c2;
	//

	while(!fim_geral){
		switch(estadoAtual){
			// ####################### SEGUE PAREDE
			case 1:
				//Envia comando I2C: Iniciando estado
				arduino.sendFunc(1,1);

				while(1){
					//Envia comando I2C: Perguntando se já acabou
					arduino.sendFunc(1,2);
					if(arduino.in[3] == 1){
						break;
					}
				}

				cout << "Acabou Estado 01." << endl;
				estadoAtual = 2;
				//fim_geral = true;
			break;
			
			// ####################### PROCURA COPO
			case 2:
				//Envia comando I2C: Iniciando estado
				arduino.sendFunc(2,1);

				while(1){
					//Envia comando I2C: Perguntando se já acabou
					arduino.sendFunc(2,2);
					if(arduino.in[3] == 1){
						break;
					}
				}

				cout << "Acabou Estado 02." << endl;
				estadoAtual = 3;
				// fim_geral = true;
			break;
			
			// ####################### PEGA COPO
			case 3:
				//Envia comando I2C: Iniciando estado
				arduino.sendFunc(3,1);

				while(1){
					//Envia comando I2C: Perguntando se já acabou
					arduino.sendFunc(3,2);
					if(arduino.in[3] == 1){
						break;
					}
					//

				}

				cout << "Acabou Estado 03." << endl;
				estadoAtual = 4;
				// fim_geral = true;
			break;

			// ####################### PROCURA VACA
			case 4:	//Procura vaca:
				
				//Envia comando I2C: Iniciando estado
				arduino.sendFunc(4,1);
				//

				//TESTES:
				// arduino.sendFunc(4,2,1);
				// cout << "Gira!" << endl;
				// usleep(1000000);
				// arduino.sendFunc(4,2,1);
				// cout << "Gira! 2" << endl;
				// usleep(1000000);
				// arduino.sendFunc(4,2,2);
				// cout << "Para de girar" << endl;
				// usleep(1000000);

				cin >> teste ;

				arduino.sendFunc(4,3,2,(unsigned char)teste);
				cout << "Manobra ai" << endl;
				usleep(100000);
				while(1){
					arduino.sendFunc(4,4);
					if(arduino.in[3] == 1) break;
					else cout << "N termino 2" << endl;
					usleep(500000);
				}
				cout << "Termino 2.." << endl;

				// arduino.sendFunc(4,5);
				// cout << "Anda ai" << endl;
				// usleep(100000);
				// while(1){
				// 	arduino.sendFunc(4,4);
				// 	if(arduino.in[3] == 1) break;
				// 	else cout << "N termino 3" << endl;
				// 	usleep(500000);
				// }
				// cout << "Termino 3.." << endl;


				//

				// while(1){
					

				// 	//PROCESSO OPENCV:
				// 	// if (!capture.read(frame)) {
				// 	// 	cout<<"\n Cannot read the video file. \n";
				// 	// 	break;
				// 	// }

				// 	// cow.setROI(frame);
				// 	// cow.transformImage(); 
				// 	// cow.searchSquares();

				// 	// if (cow.find()){
				// 	// 	cow.drawCenter(frame);
				// 	// 	cow.sendPID(c1,c2);
				// 	// } else {
				// 	// 	cow.sendPID(c1,c2);
				// 	// }

				// 	// namedWindow("Original", WINDOW_NORMAL);
				// 	// resizeWindow("Original", WIDTH, HEIGHT);
				// 	// imshow("Original", frame);

				// 	// if (waitKey(1) == 27){
				// 	// 	break;
				// 	// }
				// 	////

				// 	//Envia comando I2C: Movimentação
				// 	arduino.sendFunc(4,2);
				// 	//

				// 	cout << "N Acabou 04." << endl;
				// }

				//Fim de estado:
				cout << "Acabou 04." << endl;
				// estadoAtual = 5;
				fim_geral = true;
				//
			break;

			// ####################### ERRO !
			default:
				//Erro:
			break;
		}
	}
    return 0;
}
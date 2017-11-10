//Include Gerais:
#include <iostream>
#include <ctime>
#include <thread>

//Include I2C:
#include "I2C/I2C.h"
//Include Vaca:
#include "Libs/Cow.h"

//Includes Aruco:
#include <string>
#include <fstream>
#include <sstream>
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Geral
using namespace std;
using namespace cv;
using namespace aruco;

//Variaveis Vaca:
bool vacaThread();

//Variaveis Tanque:

//Variaveis I2C:
I2C arduino;

int main(int argc, char **argv){
	//Variaveis Máquina_Estado:
	int estadoAtual = 4;
	bool fim_geral = false;

	thread thread_vaca(vacaThread);

	while(1){
		switch(estadoAtual){
			// Start stage:
			case 1:
				while(1){
					if(arduino.sendFunc(1,1);){
						break;
					}
				}
				estadoAtual = 2;
			break;

			case 2:
				
			break;
		}
	}
    return 0;
}

void vacaThread(){
	while(1){

	}
}

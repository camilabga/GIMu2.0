#include "I2C.h"

int main(){
    I2C arduino;
	while(1){
		arduino.getData();
		cout << (int)arduino.buf[0] << " " << (int)arduino.buf[1] << " " << (int)arduino.buf[2] << endl;

		int val = 110;
		arduino.cmdS[0] = (char) val;
		arduino.enviarDados();

		usleep(1000000);
	}
    return 0;
}
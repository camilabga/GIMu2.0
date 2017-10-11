#include "I2C.h"

int main(){
    I2C arduino;
	while(1){
		arduino.getData();
		cout << arduino.buf[0] << endl;

		int val = 110;
		arduino.cmdS[0] = (char) val;
		arduino.enviarDados();

		usleep(1000000);
	}
    return 0;
}
#include "libi2c/pi2c.cpp"
#include <iostream>

using namespace std;

#define BYTES 10

int main(){
    Pi2c arduino(4);
    int qtdErro = 0;
    
    char buf[BYTES*4];
    for(int i=0;i<BYTES*4;i++){
		buf[i] = '\0';
	}
    char cmd[BYTES+1] = {"Fala.....;"};
     
	while(1){
		arduino.i2cWrite(cmd, BYTES);
		usleep(10000);
		if(arduino.i2cRead(buf,BYTES) < 0){
			qtdErro ++;
			cout << "Erro : " << qtdErro << endl;
			buf[(BYTES*4)-1] = '\0';
			cout << "GND Says: " << buf << endl;
			for(int i=0;i<BYTES*4;i++){
				buf[i] = '\0';
			}
		}else{		
			//buf[(BYTES*4)-1] = '\0';
			//cout << "Arduino Says: " << buf << endl;
			//for(int i=0;i<BYTES*4;i++){
			//	buf[i] = '\0';
			//}
		}
	}
    return 0;
}

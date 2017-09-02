#include "libi2c/pi2c.cpp"
#include <iostream>

using namespace std;

int main(){
    // Declarar objeto de comunicação no endereço 4
    Pi2c arduino(4);
    
    char cmd[11] = {"FalaAiARD;"};
    char buf[11];
    //Clean string
    for(int i=0;i<10;i++){
		buf[i] = '\0';
	}
    
	while(1){
		arduino.i2cWrite(cmd, 10);
		usleep(30000);
		arduino.i2cRead(buf,10);
		buf[10] = '\0';
		
		cout << "Arduino Says: " << buf << endl;	
		//Clean string
		for(int i=0;i<10;i++){
			buf[i] = '\0';
		}
	}
    return 0;
}

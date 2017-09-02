#include "libi2c/pi2c.cpp"
#include <iostream>

using namespace std;

int main(){
    Pi2c arduino(4);
    
    char buf[11];
    for(int i=0;i<10;i++){
		buf[i] = '\0';
	}
    char cmd[11] = {"FalaAiARD;"};
     
	while(1){
		arduino.i2cWrite(cmd, 10);
		usleep(30000);
		arduino.i2cRead(buf,10);
		
		buf[10] = '\0';
		cout << "GND Says: " << buf << endl;	
		for(int i=0;i<10;i++){
			buf[i] = '\0';
		}
	}
    return 0;
}

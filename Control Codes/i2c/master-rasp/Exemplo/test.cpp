#include "pi2c.h"
#include <iostream>

using namespace std;

int main(){
    Pi2c gnd(4);
    Pi2c sky(5);
    
    char buf1[11], buf2[11];
    for(int i=0;i<10;i++){
		buf1[i] = '\0';
		buf2[i] = '\0';
	}
    char cmd1[11] = {"FalaAiSKY;"};
    char cmd2[11] = {"FalaAiGND;"};
    
	while(1){
		sky.i2cWrite(cmd1, 10);
		usleep(30000);
		sky.i2cRead(buf1,10);
		
		gnd.i2cWrite(cmd2, 10);
		usleep(30000);
		gnd.i2cRead(buf2,10);
		
		buf1[10] = '\0';
		cout << "SKY Says: " << buf1 << endl;
		buf2[10] = '\0';
		cout << "GND Says: " << buf2 << endl;	
		for(int i=0;i<10;i++){
			buf1[i] = '\0';
			buf2[i] = '\0';
		}
	}
    return 0;
}

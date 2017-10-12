#ifndef I2C_H
#define I2C_H

#include "libi2c/Pi2c.h"
#include <iostream>
#include <string>
#include <fstream>  
#include <stdio.h>
#include <sys/stat.h>

using namespace std;

#define QTD_BYTES_I2C 10

class I2C{
public:
    I2C();

    char buf[QTD_BYTES_I2C*4];
    char cmdS[QTD_BYTES_I2C+1];

    void clearBuf();
    void getData();
    void enviarDados();

};



#endif
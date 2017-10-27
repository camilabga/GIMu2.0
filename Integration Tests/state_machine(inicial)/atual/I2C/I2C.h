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
#deinfe ADD_I2C 5

class I2C{
public:
    I2C();

    char in[QTD_BYTES_I2C*4];   //Buf de entrada
    char out[QTD_BYTES_I2C+1];  //Buf de saida

    //Limpa o bufers de entrada e saida
    void clearBufIn();
    void clearBufOut();
    
    //Usado para receber dados apenas (é enviando uma msg padrão de receber dados) - 10ms
    bool getData();

    //Usado para enviar dados apenas (é recebido uma msg padrão de confirmação) - 10ms
    bool sendData();

    //Usado para enviar e receber dados ao mesmo tempo 
    //( milisec - tempo de espera pode ser estipulado pelo usuário - max testado ???ms)
    bool tradeData(int milisec = 20);

    //Função para imprimir os dados recebidos, deve ser usado após um getData() ou tradeData()
    void printData();

};

#endif
//
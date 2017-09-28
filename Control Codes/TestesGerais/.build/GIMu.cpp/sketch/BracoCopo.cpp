#include "BracoCopo.h"

BracoCopo::BracoCopo(){}

BracoCopo::BracoCopo(int servoPulso, int servoGarra, int sharpGarra){
  pulso.attach(servoPulso);
  garra.attach(servoGarra);
  this->sharpGarra = sharpGarra;
}

int BracoCopo::getSharp(int porta){
    SharpIR SharpIR(porta, 1080);
    byte n = 20;
    int media, desvio;
    int x[20];
    long unsigned soma=0;
    for(int i=0;i<n;i++){
        x[i] = SharpIR.distance();
        soma += x[i];  // this returns the distance to the object you're measuring
    }
    media = (soma/n);
    soma = 0;
    for (int i =0;i< n; i++) {
      soma += (x[i]-media)*(x[i]-media);
    }

    desvio = soma/n;

    if (desvio > VALID_SHARP || media > 80) {
      return -1; // é ruido
    } else {
      return media; // n é ruido
    }
}


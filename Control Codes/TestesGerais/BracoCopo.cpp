#include "BracoCopo.h"

BracoCopo::BracoCopo(){
  pinMode(FDC_TRAS, INPUT_PULLUP);
  pinMode(FDC_FRENTE, INPUT_PULLUP);
}

BracoCopo::BracoCopo(int servoPulso, int servoGarra, int sharpGarra, Motor m){
  this->sharpGarra = sharpGarra;

  motorBraco.setPinFrente(m.getPinFrente());
  motorBraco.setPinTras(m.getPinTras());

  pinMode(FDC_TRAS, INPUT_PULLUP);
  pinMode(FDC_FRENTE, INPUT_PULLUP);
}

BracoCopo::BracoCopo(int servoPulso, int servoGarra, int sharpGarra, int mSharp_D, int mSharp_E, Motor m){
  this->sharpGarra = sharpGarra;
  this->mSharp_D = mSharp_D;
  this->mSharp_E = mSharp_E;

  motorBraco.setPinFrente(m.getPinFrente());
  motorBraco.setPinTras(m.getPinTras());

  pinMode(FDC_TRAS, INPUT_PULLUP);
  pinMode(FDC_FRENTE, INPUT_PULLUP);
}

void BracoCopo::attachMotor(Motor m){
  motorBraco.setPinFrente(m.getPinFrente());
  motorBraco.setPinTras(m.getPinTras());
}

int BracoCopo::getSharp(){
    SharpIR SharpIR(sharpGarra, 1080);
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

void BracoCopo::iniciar(){
  pulso.attach(SERVOG_PULSO);
  garra.attach(SERVOG_DEDO);
  pulso.write(POSICAO_INICIAL_PULSO);
  garra.write(POSICAO_INICIAL_GARRA);
}

void BracoCopo::tryGetTerrine(){
  iniciar();
  while (isFDC(FDC_FRENTE) /*&& (analogRead(MSH_GARRA_D) > ANALOG_SENSOR_COPO)*/) {
    motorBraco.moveMotor(255, 1);
    Serial.println(analogRead(MSH_GARRA_D));
  }
  motorBraco.moveMotor(0,0);
  Serial.println(isFDC(FDC_FRENTE));
  if (analogRead(MSH_GARRA_D) <= ANALOG_SENSOR_COPO) {
    garra.write(POSICAO_GARRA_FECHADA);
  }
}

void BracoCopo::recolherBraco(){
  while(isFDC(FDC_TRAS)){
    motorBraco.moveMotor(255, 0);
  }
  motorBraco.moveMotor(0,0);
}

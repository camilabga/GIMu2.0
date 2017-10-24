#include "SharpIR.h"

#define ir A0
#define model 1080
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)

SharpIR SharpIR(ir, model);

void setup() {
  // put your setup code here, tGP2Y0A02Yo run once:
  Serial.begin(9600);
}

void loop() {
  long unsigned soma=0;
  for(int i=0;i<50;i++)
  soma += SharpIR.distance();  // this returns the distance to the object you're measuring
  Serial.println(soma/50);
  delay(100);
}

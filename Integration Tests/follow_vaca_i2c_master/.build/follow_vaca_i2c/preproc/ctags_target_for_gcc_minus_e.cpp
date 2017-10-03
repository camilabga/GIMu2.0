# 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca_i2c/follow_vaca_i2c.ino" 2

Motor esquerdo(9 /*Esquerdo*/, 6);
Motor direito(5 /*Direito*/, 3);
GIMu robo (direito, esquerdo);




unsigned char in[10*4];
unsigned char out[10] = {"Falei....;"};

/*char in[BYTES*4];
char out[BYTES] = {"01234567;"};
int qtdErro = 0;*/

void setup() {
  Serial.begin(9600);
  Wire.setTimeout(10);
  Wire.begin(0x04);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

}

void loop() {
  /*receiveData(BYTES);
  sendData();
  delay(10);*/

  /*Serial.readBytesUntil(';', S, 7);
  if(S[0]=='r'){
      int velDir = 100;
      int velEsq = (int)S[1];
      Serial.print(velEsq);
      Serial.print(" ");
      Serial.println(velDir);
      robo.moveTank(velEsq, velDir);

  } else if (S[0]=='f') {
    robo.moveTank(-100, 100);
    Serial.println("girar");

  } else if (S[0]=='p') {
    robo.moveFrente(0);
    Serial.println("parado");
  }*/
}

void receiveData(int byteCount) {
  if(byteCount != 10){
    while(Wire.available()) {
      Wire.read();
    }
  }else{
    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);
      if(in[0]=='r'){
          int velDir = 100;
          int velEsq = (int)in[1];
          Serial.print(velEsq);
          Serial.print(" ");
          Serial.println(velDir);
          robo.moveTank(velEsq, velDir);

      } else if (in[0]=='f') {
        robo.moveTank(-100, 100);
        Serial.println("girar");

      } else if (in[0]=='p') {
        robo.moveFrente(0);
        Serial.println("parado");
      }
    }

    for(int i=0;i<10*4;i++){
      in[i] = '\0';
    }
  }
}

void sendData() {
  Wire.write(out, 10);
}

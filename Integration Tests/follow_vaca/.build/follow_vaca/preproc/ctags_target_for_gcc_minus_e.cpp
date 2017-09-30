# 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Integration Tests/follow_vaca/follow_vaca.ino" 2

Motor esquerdo(9 /*Esquerdo*/, 6);
Motor direito(5 /*Direito*/, 3);
GIMu robo (direito, esquerdo);




char in[10*4];
char out[10] = {"Falei....;"};
int qtdErro = 0;

void setup() {
  Serial.begin(9600);
  Wire.setTimeout(10);
  Wire.begin(0x04);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.begin(9600);
}

void loop() {
  receiveData(10);
  sendData();
}

void receiveData(int byteCount) {
  if(byteCount != 10){
    while(Wire.available()) {
      Wire.read();
    }
    qtdErro++;
    Serial.print("Erro : ");
    Serial.print(qtdErro);
    Serial.print(" - Qtd Bytes: ");
    Serial.println(byteCount);
  }else{
    while (Wire.available()) {
      Wire.readBytesUntil(';', in, byteCount);

      if (in[0] == 1) {
        robo.moveTank(in[1], in[2]);
        Serial.println("mexe p um lado");
      } else if (in[0] == 0) {
        Serial.println("para");
        robo.moveFrente(0);
      } else if (in[0] == 2) {
        Serial.println("procura");
        robo.moveTank(-200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/, 200 /* velocidade para seguir em frente como se n houvesse amanha (ou parede)*/);
      }
    }
//    Serial.println(in);
//    for(int i=0;i<BYTES*4;i++){
//      in[i] = '\0';
//    }
  }
}

void sendData() {
  Wire.write(out, 10);
}

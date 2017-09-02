
#include <Wire.h>

#define SLAVE_ADDRESS 0x04
char in[10];
char out[10] = {"EuSouGND.."};

void setup() {
  Wire.setTimeout(100);
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop() {
  delay(100);
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    Wire.readBytesUntil(';', in, 10);
  }
}

void sendData() {
  Wire.write(out, 10);
}

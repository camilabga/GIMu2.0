
//Configurações de pinos da roda direita.
int r1 = 3;
int r2 = 4;
int e1 = 9; // Enable

#define VELOCIDADE_BASE  100

//Configurações de pinos da roda esquerda.
int l1 = 5;
int l2 = 6;
int e2 = 10; // Enable

//Funções de movimentação.

void go();

int loops = 0;
unsigned char S[10];
int velEsq, velDir;


void setup() {
  //Definindo as portas como saída.
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(e1, OUTPUT);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(e2, OUTPUT);
  pinMode(13, OUTPUT);
  //Ativando as rodas:


  Serial.begin(9600);

}

void loop() {
  digitalWrite(13, 1);
  analogWrite(e1, 100);
  analogWrite(e2, 100);
  go(1);
  delay(2000);
  
  digitalWrite(13, 0);
  
  go(-1);
  delay(100);
  
  analogWrite(e1, 50);
  analogWrite(e2, 100);
  go(1);
  delay(1000);

  go(-1);
  delay(100);
  
  analogWrite(e1, 100);
  analogWrite(e2, 50);
  go(1);
  delay(1000);

}
/*void go() {
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
}*/
void go_back() {
  digitalWrite(r1, 0);
  digitalWrite(r2, 1);
  digitalWrite(l1, 0);
  digitalWrite(l2, 1);
}
void go(int sentido) { //frente =1 tras=-1 para =0
  if (sentido == 1) {
    digitalWrite(r1, 1);
    digitalWrite(r2, 0);
    digitalWrite(l1, 1);
    digitalWrite(l2, 0);
  }
  if (sentido == 0) {
    digitalWrite(r1, 0);
    digitalWrite(r2, 0);
    digitalWrite(l1, 0);
    digitalWrite(l2, 0);
  }
  if (sentido == -1) {
    digitalWrite(r1, 0);
    digitalWrite(r2, 1);
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
  }

}




//Configurações de pinos da roda direita.
int r1 = 3;
int r2 = 4;
int e1 = 9; // Enable

#define VELOCIDADE_BASE  160

//Configurações de pinos da roda esquerda.
int l1 = 5;
int l2 = 6;
int e2 = 10; // Enable

//Funções de movimentação.

void left();
void right();

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
  //Ativando as rodas:


  Serial.begin(9600);

}

void loop() {
  Serial.readBytesUntil(';', S, 7); // leitura do segundo dado (pd no eixo x) -> esse já vem normalizado, basta ajustar as unidades
  if (S[0] == 'r') {
    int vel_pd = (int)S[1];
    Serial.println((int)S[1]);
    vel_pd = vel_pd - 100;
    float vel_normal = vel_pd / 100.0;
    if (vel_normal >= 0) {
      velEsq = VELOCIDADE_BASE;
      velDir = (int)(velEsq * (1 - vel_normal));
      analogWrite(e1, velDir);
      analogWrite(e2, velEsq);
      go();

    } else {
      vel_normal = vel_normal * -1;
      velDir = VELOCIDADE_BASE;
      velEsq = (int)(velDir * (1 - vel_normal));
      analogWrite(e2, velDir);
      analogWrite(e1, velEsq);
      go();
    }

  } else if (S[0] == 'p') {
    digitalWrite(e2, 0);
    digitalWrite(e1, 0);
  }
}

void go() {
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
}



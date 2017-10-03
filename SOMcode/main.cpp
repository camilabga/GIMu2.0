#include "libi2c/pi2c.cpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <SOM.h>
#include <sys/stat.h>

using namespace std;

#define BYTES 10
void traingSOM(int size, std::string filename) {

  // setando posiçoes de leitura e escrita
  std::string subOutput = "output/" + filename + "Size:" + std::to_string(size);
  std::string dataFile = "Coleta/" + filename + ".csv";
  std::string csvHeader;

  // criando diretório
  mkdir("output", 0777);
  mkdir(subOutput.c_str(), 0777);

  SOM som(size);

  DataSet *data = new DataSet(dataFile);
  data->show();
  std::string outputFile = subOutput + "/output";
  som.setDataSet(data);
  // std::cout <<  outputFile << std::endl;

  float maxFeatureInitialValue = 0.01;
  // std::cout<< "data:: " << data->getSampleSize() << std::endl;
  som.initializeNodes(data->getSampleSize(), true, maxFeatureInitialValue);

  int iterations = 20000;

  // Execute many iterations
  int i = 0;

  csvHeader = "x,y," + data->features;

  som.saveNodes(outputFile, csvHeader.c_str(), false);
  while (i < iterations) {
    som.executeOneIt();
    i++;
    if (i % 1000 == 0)
      som.saveNodes(outputFile, csvHeader.c_str(), false);
  }
  std::cout << "Iteractions executed: " << iterations << std::endl;

  delete data;
}

void logCsv(std::string data, std::string filename, std::string header) {
  ofstream myfile;

  while (true) {
    myfile.open(filename.c_str(), ios::in);
    if (myfile.is_open() && myfile.good()) {
      myfile.close();
      myfile.open(filename.c_str(), ios::app);
      myfile << data << endl;
      return;
    } else {
      myfile.open(filename.c_str(), ios::app);
      if (myfile.is_open() && myfile.good()) {
        myfile << header << endl;
        myfile.close();
      }
    }
  }
}

int main() {
  string data = "";
  string csvHeader =
      "motor1,motor2,sensor1,sensor2,sensor3,sensor4,sensor5,sensor6";
  string filename = "teste.csv";
  Pi2c arduino(4);
  int qtdErro = 0;

  char buf[BYTES * 4];
  for (int i = 0; i < BYTES * 4; i++) {
    buf[i] = '\0';
  }

  char cmdF[BYTES + 1] = {"F........;"};
  char cmdT[BYTES + 1] = {"T........;"};
  char cmdD[BYTES + 1] = {"D........;"};
  char cmdE[BYTES + 1] = {"E........;"};
  char cmdI[BYTES + 1] = {"I........;"};
  char cmdS[BYTES + 1];

  while (1) {
    char input = getchar();
    switch (input) {
    case 'w':
      for (int i = 0; i < BYTES + 1; i++) {
        cmdS[i] = cmdF[i];
      }
      data += "150,150";
      break;
    case 's':
      for (int i = 0; i < BYTES + 1; i++) {
        cmdS[i] = cmdT[i];
      }
      data += "-150,-150";
      break;
    case 'd':
      for (int i = 0; i < BYTES + 1; i++) {
        cmdS[i] = cmdD[i];
      }
      data += "150,-150";
      break;
    case 'a':
      for (int i = 0; i < BYTES + 1; i++) {
        cmdS[i] = cmdE[i];
      }
      data += "-150,150";
      break;
    case 'i':
      for (int i = 0; i < BYTES + 1; i++) {
        cmdS[i] = cmdI[i];
      }
      break;
    default:
      return 0;
      break;
    }
    cin.ignore();

    // Comando para Andar:
    arduino.i2cWrite(cmdS, BYTES);
    usleep(10000);

    if (arduino.i2cRead(buf, BYTES) == BYTES) {
      buf[(BYTES * 4) - 1] = '\0';
    } else {
      cout << "Erro : " << endl;
    }
    for (int i = 0; i < BYTES * 4; i++) {
      buf[i] = '\0';
    }

    // Receber Dados:
    usleep(1100000);
    for (int i = 0; i < BYTES + 1; i++) {
      cmdS[i] = cmdI[i];
    }
    arduino.i2cWrite(cmdS, BYTES);
    usleep(10000);

    int aux;
    if (arduino.i2cRead(buf, BYTES) == BYTES) {
      buf[(BYTES * 4) - 1] = '\0';
      for (int i = 0; i < 6; i++) {
        cout << " " << i << ": " << (int)buf[i];
        aux = (int)buf[i];
        data += "," + to_string(aux);
      }
      /*
      0 e 1 -> Frente
      2 e 3 -> Esquerda
      4 e 5 -> Direita
      */
      cout << endl;

      logCsv(data.c_str(), filename.c_str(), csvHeader.c_str());
      data.clear();
    } else {
      cout << "Erro : " << endl;
    }
    for (int i = 0; i < BYTES * 4; i++) {
      buf[i] = '\0';
    }
  }
  return 0;
}

//#include "libi2c/pi2c.cpp"
#include <I2C.h>
#include <SOM.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
using namespace std;

#define BYTES 10

void traingSOM(int size, std::string filename);
void logCsv(std::string data, std::string filename, std::string header);
void collectDataforNetWork(std::string filename);
void seguirParedeSOM(std::string output);
void teclado(I2C &arduino, std::string &data);

int main() {
  //collectDataforNetWork("Coleta/teste3.csv");
  seguirParedeSOM("output20000.csv");

  return 0;
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

void collectDataforNetWork(std::string filename) {
  string data = "";
  string csvHeader =
      "motor1,motor2,sensor1,sensor2,sensor3,sensor4,sensor5,sensor6";

  I2C arduino;
  // Pi2c arduino(4);
  arduino.cmdS[9] = ';';

  while (1) {
   
    teclado (arduino,data);
    // Comando para Andar:
    arduino.sendData();
    usleep(1100000);

    // Receber Dados:
    arduino.getData();
    int aux;

    for (int i = 0; i < 6; i++) {
      cout << " " << i << ": " << (int)arduino.buf[i];
      aux = (int)arduino.buf[i];
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
  }
}
void seguirParedeSOM(std::string output) {

  SOM som(30);
  som.loadNodes(output.c_str());
  I2C arduino;

  std::vector<double> input{0, 0, 0, 0, 0, 0, 0, 0};

  while (true) {
    arduino.getData();
    int aux;
    int val;
    std::cout << "Sensor: ";
    for (int i = 0; i < 6; i++) {

      aux = (int)arduino.buf[i];
      std::cout << aux << " ";
      input[i] = aux;
    }
    std::cout << std::endl;
    som.findBest(input, 0, 5);


    aux = (int)input[0];
    val = (int)input[1];


    

    val /= 2;
    aux /= 2;
    std::cout << "Saida: " << val << " " << aux << std::endl;
    arduino.cmdS[0] = (char)val;
    arduino.cmdS[1] = (char)val;

    arduino.sendData();
    /*
    0 e 1 -> Frente
    2 e 3 -> Esquerda
    4 e 5 -> Direita
    */
  }

}

void teclado (I2C &arduino, std::string &data){
  char input = getchar();
  switch (input) {
  case 'w':
    arduino.cmdS[0] = 'F';
    data += "150,150";
    break;
  case 's':
    arduino.cmdS[0] = 'T';
    data += "-150,-150";
    break;
  case 'd':
    arduino.cmdS[0] = 'D';
    data += "150,-150";
    break;
  case 'a':
    arduino.cmdS[0] = 'E';
    data += "-150,150";
    break;
  case 'i':
    arduino.cmdS[0] = 'I';
    break;
  default:
    return;
    break;
  }
  cin.ignore();

}

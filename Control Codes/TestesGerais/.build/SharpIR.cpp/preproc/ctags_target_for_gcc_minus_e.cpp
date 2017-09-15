# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/SharpIR.cpp"
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/SharpIR.cpp"
/*
	SharpIR

	Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK

	From an original version of Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)     
	
    Version : 1.0 : Guillaume Rico
    + Remove average and use median
    + Definition of number of sample in .h
    + Define IR pin as input

    Version : 1.1 : Thibaut Mauon
    + Add SHARP GP2Y0A710K0F for 100cm to 500cm by Thibaut Mauron

	https://github.com/guillaume-rico/SharpIR
    
    Original comment from Dr. Marcal Casas-Cartagena :
   The Sahrp IR sensors are cheap but somehow unreliable. I've found that when doing continous readings to a
   fix object, the distance given oscilates quite a bit from time to time. For example I had an object at
   31 cm. The readings from the sensor were mainly steady at the correct distance but eventually the distance
   given dropped down to 25 cm or even 16 cm. That's quite a bit and for some applications it is quite
   unacceptable. I checked the library http://code.google.com/p/gp2y0a21yk-library/ by Jeroen Doggen
   (jeroendoggen@gmail.com) and what the author was doing is to take a bunch of readings and give an average of them

   The present library works similary. It reads a bunch of readings (avg), it checks if the current reading
   differs a lot from the previous one (tolerance) and if it doesn't differ a lot, it takes it into account
   for the mean distance.
   The distance is calculated from a formula extracted from the graphs on the sensors datasheets
   After some tests, I think that a set of 20 to 25 readings is more than enough to get an accurate distance
   Reading 25 times and return a mean distance takes 53 ms. For my application of the sensor is fast enough.
   This library has the formulas to work with the GP2Y0A21Y and the GP2Y0A02YK sensors but exanding it for
   other sensors is easy enough.
*/







# 43 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/SharpIR.cpp" 2

// Initialisation function
//  + irPin : is obviously the pin where the IR sensor is attached
//  + sensorModel is a int to differentiate the two sensor models this library currently supports:
//    > 1080 is the int for the GP2Y0A21Y and 
//    > 20150 is the int for GP2Y0A02YK and 
//    > 100500 is the long for GP2Y0A710K0F
//    The numbers reflect the distance range they are designed for (in cm)
SharpIR::SharpIR(int irPin, long sensorModel) {

    _irPin=irPin;
    _model=sensorModel;

    // Define pin as Input
    pinMode (_irPin, 0x0);


      analogReference(1);

}

// Sort an array
void SharpIR::sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        bool flag = true;
        for(int o=0; o<(size-(i+1)); o++) {
            if(a[o] > a[o+1]) {
                int t = a[o];
                a[o] = a[o+1];
                a[o+1] = t;
                flag = false;
            }
        }
        if (flag) break;
    }
}

// Read distance and compute it
int SharpIR::distance() {

    int ir_val[25];
    int distanceCM;
    float current;


    for (int i=0; i<25; i++){
        // Read analog value
        ir_val[i] = analogRead(_irPin);
    }

    // Sort it 
    sort(ir_val,25);


    if (_model==1080) {

        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos

          distanceCM = 27.728 * pow(map(ir_val[25 / 2], 0, 1023, 0, 5000)/1000.0, -1.2045);




    } else if (_model==20150){

        // Previous formula used by  Dr. Marcal Casas-Cartagena
        // puntualDistance=61.573*pow(voltFromRaw/1000, -1.1068);

        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos

          distanceCM = 60.374 * pow(map(ir_val[25 / 2], 0, 1023, 0, 5000)/1000.0, -1.16);




    } else if (_model==430){

        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos

          distanceCM = 12.08 * pow(map(ir_val[25 / 2], 0, 1023, 0, 5000)/1000.0, -1.058);




    } else if (_model==100500){


          current = map(ir_val[25 / 2], 0, 1023, 0, 5000);



        // use the inverse number of distance like in the datasheet (1/L)
        // y = mx + b = 137500*x + 1125 
        // x = (y - 1125) / 137500
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        if (current < 1400 || current > 3300) {
          //false data
          distanceCM = 0;
        } else {
          distanceCM = 1.0 / (((current - 1125.0) / 1000.0) / 137.5);
        }
    }

    return distanceCM;
}
# 1 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino"
# 2 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2
# 3 "/home/barbosa/Documentos/GIMu 2.0/Control Codes/TestesGerais/TestesGerais.ino" 2

Motor esquerdo(3 /*Esquerdo*/, 5);
Motor direito(6 /*Direito*/, 9);
GIMu robo (direito, esquerdo);

void setup() {
  //Serial.begin(9600);
}

void loop() {
  // ### Teste de Movimentação:
  // robo.moveFrente(255);
  // delay(1000);
  // robo.moveTras(255);
  // delay(1000);
  // robo.moveTank(120, 240);
  // delay(1000);
  // ###

  // ### Teste dos sensores Sharps:
  /*Serial.print(" S0: ");
  Serial.print(robo.getSharp(SH0));
  Serial.print(" S1: ");
  Serial.print(robo.getSharp(SH1));
  Serial.print(" S2: ");
  Serial.print(robo.getSharp(SH2));
  Serial.print(" S3: ");
  Serial.print(robo.getSharp(SH3));
  Serial.print(" S4: ");
  Serial.print(robo.getSharp(SH4));
  Serial.print(" S5: ");
  Serial.println(robo.getSharp(SH5));
  delay(500);*/
  // ###


  robo.follow_wall_to_cup();

}

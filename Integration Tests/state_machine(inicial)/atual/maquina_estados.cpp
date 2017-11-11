#include <iostream>
#include <ctime>
#include <thread>

//Include I2C:
#include "I2C/I2C.h"

//Include Vaca:
#include "Libs/Cow.h"

//Includes Aruco:
#include <string>
#include <fstream>
#include <sstream>
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Geral//
using namespace std;
using namespace cv;
using namespace aruco;

//Variaveis Vaca:
bool vaiVaca();
volatile int acaoVaca;
volatile bool acabaVaca = false;

//Variaveis Aruco:
#define HEIGHT 480
#define WIDTH 640
#define LOOKING_SPEED 200
#define TURNING_SPEED 150
bool vaiAruco(int argc, char **argv);
float erro;
int velE, velD;

MarkerDetector MDetector;
VideoCapture TheVideoCapturer;
vector< Marker > TheMarkers;
Mat TheInputImage, TheInputImageCopy;
CameraParameters TheCameraParameters;
void cvTackBarEvents(int pos, void *);

pair< double, double > AvrgTime(0, 0); // determines the average time required for detection
int iThresParam1, iThresParam2;
int waitTime = 0;
class CmdLineParser{
   int argc; 
   char **argv; 
   
public: 
   CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}  
   bool operator[] ( string param ) {int idx=-1;  
	   for ( int i=0; i<argc && idx==-1; i++ ) 
	   if ( string ( argv[i] ) ==param ) idx=i;    
	   return ( idx!=-1 ) ;    
   } 
   
   string operator()(string param,string defvalue="-1"){
	   int idx=-1;    
	   for ( int i=0; i<argc && idx==-1; i++ ) 
	   if ( string ( argv[i] ) ==param ) idx=i; 
	   if ( idx==-1 ) return defvalue;   
	   else  return ( argv[  idx+1] ); 
   }
};
cv::Mat resize(const cv::Mat &in,int width){
   if (in.size().width<=width) return in;
   float yf=float(  width)/float(in.size().width);
   cv::Mat im2;
   cv::resize(in,im2,cv::Size(width,float(in.size().height)*yf));
   return im2;

}

//Variaveis I2C:
I2C arduino;

int main(int argc, char **argv){
	//Variaveis Máquina_Estado:
	int estadoAtual = 3;
	bool fim_geral = false;

	thread thread_vaca(vaiVaca);

	while(1){
		while(!fim_geral){
			switch(estadoAtual){
				// ####################### SEGUE PAREDE
				case 1:
					//Envia comando I2C: Iniciando estado
					arduino.sendFunc(1,1);

					while(1){
						//Envia comando I2C: Perguntando se já acabou
						arduino.sendFunc(1,2);
						if(arduino.in[2] == 1){
							break;
						}
					}

					cout << "Acabou Estado 01." << endl;
					estadoAtual = 2;
					//fim_geral = true;
				break;
				
				// ####################### PROCURA COPO
				case 2:
					//Envia comando I2C: Iniciando estado
					arduino.sendFunc(2,1);

					while(1){
						//Envia comando I2C: Perguntando se já acabou
						arduino.sendFunc(2,2);
						if(arduino.in[2] == 1){
							break;
						}
					}

					cout << "Acabou Estado 02." << endl;
					// estadoAtual = 3;
					fim_geral = true;
				break;
				
				// ####################### PEGA COPO
				case 3:
					//Envia comando I2C: Iniciando estado
					arduino.sendFunc(3,1);

					while(1){
						//Envia comando I2C: Perguntando se já acabou
						arduino.sendFunc(3,2);
						if(arduino.in[2] == 1){
							break;
						}
					}
					cout << "Acabou Estado 03." << endl;
					estadoAtual = 4;
					// fim_geral = true;
				break;

				// ####################### PROCURA VACA
				case 4:	//Procura vaca:
					
					//Envia comando I2C: Iniciando estado
					arduino.sendFunc(4,1);
					cout << "Inicio estado 4" << endl;
					//
					// vaiVaca();//
					while(!acabaVaca){
						cout << acabaVaca << endl;
						switch(acaoVaca){
							case 1:
								// cout << "Girar loucamente" << endl;
								// acaoVaca = 1;
								arduino.sendFunc(4,2,1);
								usleep(70000);
								arduino.sendFunc(4,2,2);
								cout << "SE1" << endl;
							break;

							case 2:
								// cout << "Girar Controlado" << endl;
								// acaoVaca = 2;
								arduino.sendFunc(4,3,velE,velD);
								usleep(80000);
								arduino.sendFunc(4,3,0,0);
								cout << "SE2" << endl;
							break;

							case 3:
								// cout << "Tenta alinhar" << endl;
								// acaoVaca = 3;
								arduino.sendFunc(4,4,2,20);
								while(1){
									arduino.sendFunc(4,5);
									if(arduino.in[2] == 1){
										break;
									}
									usleep(200000);
									cout << "SE3" << endl;
								}
							break;

							case 4:
								// cout << "Tenta alinhar" << endl;
								// acaoVaca 4;
								arduino.sendFunc(4,4,1,20);
								while(1){
									arduino.sendFunc(4,5);
									if(arduino.in[2] == 1){
										break;
									}
									usleep(200000);
									cout << "SE4" << endl;
								}
							break;

							case 5:
								// cout << " Vai em frente" << endl;
								// acaoVaca = 5;
								arduino.sendFunc(4,6);
								while(1){
									arduino.sendFunc(4,7);
									if(arduino.in[2] == 1){
										break;
									}
									usleep(200000);
								}
								cout << "SE2" << endl;
								acabaVaca = true;
							break;
							default:
								usleep(2000000);
							break;
						}
						usleep(100000);
					}
					//Fim de estado:
					cout << "Acabou Estado 04." << endl;
					estadoAtual = 5;
					// fim_geral = true;
					//
				break;

				case 5:
					//Envia comando I2C: Iniciando estado
					arduino.sendFunc(5,1);

					while(1){
						//Envia comando I2C: Perguntando se já acabou
						arduino.sendFunc(5,2);
						if(arduino.in[2] == 1){
							break;
						}
						//
					}
					cout << "Acabou Estado 05." << endl;
					estadoAtual = 6;
					// fim_geral = true;
				break;

				case 6:
					//Envia comando I2C: Iniciando estado
					arduino.sendFunc(6,1);

					while(1){
						//Envia comando I2C: Perguntando se já acabou
						arduino.sendFunc(6,2);
						if(arduino.in[2] == 1){
							break;
						}

					}
					cout << "Acabou Estado 06." << endl;
					estadoAtual = 7;
					// fim_geral = true;
				break;

				case 7:
					vaiAruco(argc, argv);

					cout << "Acabou Estado 07." << endl;
					// estadoAtual = 9;
					fim_geral = true;
				break;

				case 8:
					//Envia comando I2C: Iniciando estado
					arduino.sendFunc(8,1);

					while(1){
						//Envia comando I2C: Perguntando se já acabou
						arduino.sendFunc(8,2);
						if(arduino.in[2] == 1){
							break;
						}
					}
					cout << "Acabou Estado 08." << endl;
					// estadoAtual = 9;
					fim_geral = true;
				break;

				// ####################### ERRO !
				default:
					//Erro:
				break;
			}
		}
		arduino.sendFunc(20,1);
		if(arduino.in[2] == 1){
			fim_geral=false;
		}
	}
    return 0;
}

bool vaiVaca(){
	//open webcam
    //VideoCapture capture("arena.mp4");
    VideoCapture capture(0);
    if ( !capture.isOpened() ){
      cout << "Cannot open the video file" << endl;
      return -1;
	}
	// capture.set(CV_CAP_PROP_BUFFERSIZE, 1);
	
	
    /*create Cow -> first initialization has no center
    and the still scans the whole Mat, also, do not contain
    any rectangle center defined */

    Cow cow;
    Mat frame;

	
    while (!acabaVaca){
        if (!capture.read(frame)) {
            cout<<"\n Cannot read the video file. \n";
            break;
        }
		
		// flip(frame,frame,-1);

        cow.setROI(frame);
        cow.transformImage(); 
        cow.searchSquares();
 
        if (cow.find()){
            cow.detectLimits();
            if (cow.isCentered()) {
                if (cow.isAlign()) {
					cout << " Vai em frente" << endl;
					acaoVaca = 5;
					velE = LOOKING_SPEED;
                    velD = LOOKING_SPEED;
					//VAI FRENTE
					// arduino.sendFunc(4,6);
					// while(1){
					// 	arduino.sendFunc(4,7);
					// 	if(arduino.in[2] == 1){
					// 		break;
					// 	}
					// 	usleep(200000);
					// }
					// acabaVaca = true;
                } else {
                    // ALINHAR 180 GRAUS COM A VACA
                    if (cow.getSlope() > 100) {
						cout << "Tenta alinhar" << endl;
						acaoVaca = 4;
						// arduino.sendFunc(4,4,1,(int) (180 - cow.getSlope()));
						// while(1){
						// 	arduino.sendFunc(4,5);
						// 	if(arduino.in[2] == 1){
						// 		break;
						// 	}
						// 	usleep(200000);
						// }
						cow.restartLooking();
                    } else {
						cout << "Tenta alinhar" << endl;
						acaoVaca = 3;
						// arduino.sendFunc(4,4,2,(int) (cow.getSlope()));
						// while(1){
						// 	arduino.sendFunc(4,5);
						// 	if(arduino.in[2] == 1){
						// 		break;
						// 	}
						// 	usleep(200000);
						// }
						cow.restartLooking();
                    }
                }
            } else {
				cout << "Girar Controlado" << endl;
				acaoVaca = 2;

				if (cow.center.x > 320) {
					velE = -TURNING_SPEED;
					velD = TURNING_SPEED;
				} else {
					velE = TURNING_SPEED;
					velD = -TURNING_SPEED;
				}
                
				// arduino.sendFunc(4,3,velD,velE);
				// // tempo_delay_descarte = time(NULL);
				// // while(time(NULL) >= tempo_delay_descarte + 2){
				// // 	capture.read(frame);
				// // }
				// usleep(100000);
				// // capture.grab(); 
				// arduino.sendFunc(4,3,0,0);
            }
        } else {
			// GIRAR LOUCAMENTE
			cout << "Girar loucamente" << endl;
			acaoVaca = 1;
			// arduino.sendFunc(4,2,1);
			// // tempo_delay_descarte = time(NULL);
			// // while(time(NULL) >= tempo_delay_descarte + 2){
			// // 	capture.read(frame);
			// // }
			// usleep(100000);
			// // capture.grab();
			// arduino.sendFunc(4,2,2);
        }

        namedWindow("Original", WINDOW_NORMAL);
        resizeWindow("Original", WIDTH, HEIGHT);
        imshow("Original", frame);

        if (waitKey(1) == 27){
            break;
        }
    }
    capture.release();
    return 0;
}

bool vaiAruco(int argc, char **argv){
	try {
        CmdLineParser cml(argc,argv);
        if (argc < 2  || cml["-h"]) {
            cerr << "Invalid number of arguments" << endl;
            cerr << "Usage: (in.avi|live[:idx_cam=0]) [-c camera_params.yml] [-s  marker_size_in_meters] [-d dictionary:ARUCO by default] [-h]" << endl;
            cerr<<"\tDictionaries: "; for(auto dict:aruco::Dictionary::getDicTypes())    cerr<<dict<<" ";cerr<<endl;
            cerr<<"\t Instead of these, you can directly indicate the path to a file with your own generated dictionary"<<endl;
            return false;
        }

        ///////////  PARSE ARGUMENTS
        string TheInputVideo = argv[1];
        // read camera parameters if passed
        if (cml["-c"] )  TheCameraParameters.readFromXMLFile(cml("-c"));
        float TheMarkerSize = std::stof(cml("-s","-1"));
        //aruco::Dictionary::DICT_TYPES  TheDictionary= Dictionary::getTypeFromString( cml("-d","ARUCO") );

        ///////////  OPEN VIDEO
        // read from camera or from  file
        if (TheInputVideo.find("live") != string::npos) {
            int vIdx = 0;
            // check if the :idx is here
            char cad[100];
            if (TheInputVideo.find(":") != string::npos) {
                std::replace(TheInputVideo.begin(), TheInputVideo.end(), ':', ' ');
                sscanf(TheInputVideo.c_str(), "%s %d", cad, &vIdx);
            }
            cout << "Opening camera index " << vIdx << endl;
            TheVideoCapturer.open(vIdx);
            waitTime = 10;
        }
        else TheVideoCapturer.open(TheInputVideo);
        // check video is open
        if (!TheVideoCapturer.isOpened())  throw std::runtime_error("Could not open video");


        ///// CONFIGURE DATA
        // read first image to get the dimensions
        TheVideoCapturer >> TheInputImage;
        if (TheCameraParameters.isValid())
            TheCameraParameters.resize(TheInputImage.size());

        MDetector.setDictionary(cml("-d","ARUCO"));//sets the dictionary to be employed (ARUCO,APRILTAGS,ARTOOLKIT,etc)
        MDetector.setThresholdParams(7, 7);
        MDetector.setThresholdParamRange(2, 0);
       //  MDetector.setCornerRefinementMethod(aruco::MarkerDetector::SUBPIX);

        //gui requirements : the trackbars to change this parameters
        iThresParam1 = MDetector.getParams()._thresParam1;
        iThresParam2 = MDetector.getParams()._thresParam2;
        cv::namedWindow("in");
        cv::createTrackbar("ThresParam1", "in", &iThresParam1, 25, cvTackBarEvents);
        cv::createTrackbar("ThresParam2", "in", &iThresParam2, 13, cvTackBarEvents);

        //go!
        char key = 0;
        int index = 0;
		bool found_tag = false, ja_achou = false;

		arduino.sendFunc(7,1);
		
		// capture until press ESC or until the end of the video
        do {
            found_tag = false;
            TheVideoCapturer.retrieve(TheInputImage);
            // copy image
            double tick = (double)getTickCount(); // for checking the speed
            // Detection of markers in the image passed
            TheMarkers= MDetector.detect(TheInputImage, TheCameraParameters, TheMarkerSize);
            // chekc the speed by calculating the mean speed of all iterations
            AvrgTime.first += ((double)getTickCount() - tick) / getTickFrequency();
            AvrgTime.second++;
            //cout << "\rTime detection=" << 1000 * AvrgTime.first / AvrgTime.second << " milliseconds nmarkers=" << TheMarkers.size() << std::endl;

            // print marker info and draw the markers in image
            TheInputImage.copyTo(TheInputImageCopy);

            for (unsigned int i = 0; i < TheMarkers.size(); i++) {
                if (TheMarkers[i].id == 213) {
                    ja_achou = true;
                    found_tag = true;
                    if (TheMarkers[i].getCenter().x < WIDTH/2) {
                        erro = (WIDTH/2 - TheMarkers[i].getCenter().x)/(WIDTH/2);

                        velD = LOOKING_SPEED;
                        velE = LOOKING_SPEED*erro;

						// VIRAR PRA DIREITA
						arduino.sendFunc(7,3,velE,velD);
						
						if(arduino.in[2] == 1){
							arduino.sendFunc(7,4);
							while(1){
								arduino.sendFunc(7,5);
								if(arduino.in[2] == 1){
									break;
								}
								usleep(200000);
							}
						}
                    } else if (TheMarkers[i].getCenter().x == WIDTH/2) {
                        velE = LOOKING_SPEED;
                        velD = LOOKING_SPEED;

						arduino.sendFunc(7,3,velE,velD);

						if(arduino.in[2] == 1){
							arduino.sendFunc(7,4);
							while(1){
								arduino.sendFunc(7,5);
								if(arduino.in[2] == 1){
									break;
								}
								usleep(200000);
							}
						}
						
                    } else {
                        erro = (TheMarkers[i].getCenter().x - WIDTH/2)/(WIDTH/2);
                        
                        velD = LOOKING_SPEED*erro;
                        velE = LOOKING_SPEED;
                        
						// VAI PRA ESQUERDA
						arduino.sendFunc(7,3,velE,velD);
						
						if(arduino.in[2] == 1){
							arduino.sendFunc(7,4);
							while(1){
								arduino.sendFunc(7,5);
								if(arduino.in[2] == 1){
									break;
								}
							}
							usleep(200000);
						}
                    }

                    cout << TheMarkers[i].getCenter() << endl;

                }
                //cout << TheMarkers[i]<<endl;
                TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255));
            }

            if (!ja_achou) {
                arduino.sendFunc(7,2,1);
                velE = TURNING_SPEED;
                velD = -TURNING_SPEED;

            } else if (!found_tag) {
                arduino.sendFunc(7,3,0,0);

                velE = 0;
                velE = 0;
            }

            // draw a 3d cube in each marker if there is 3d info
            if (TheCameraParameters.isValid() && TheMarkerSize>0)
                for (unsigned int i = 0; i < TheMarkers.size(); i++) {
                    CvDrawingUtils::draw3dCube(TheInputImageCopy, TheMarkers[i], TheCameraParameters);
                    CvDrawingUtils::draw3dAxis(TheInputImageCopy, TheMarkers[i], TheCameraParameters);
                }

            // DONE! Easy, right?
            // show input with augmented information and  the thresholded image
            cv::imshow("in", resize(TheInputImageCopy,1280));
            cv::imshow("thres", resize(MDetector.getThresholdedImage(),1280));


            key = cv::waitKey(waitTime); // wait for key to be pressed
            if(key=='s')  waitTime= waitTime==0?10:0;
            index++; // number of images captured
        } while (key != 27 && (TheVideoCapturer.grab() ));

    } catch (std::exception &ex)

    {
        cout << "Exception :" << ex.what() << endl;
	}
	return false;
}

void cvTackBarEvents(int pos, void *) {
    (void)(pos);
    if (iThresParam1 < 3)  iThresParam1 = 3;
    if (iThresParam1 % 2 != 1)  iThresParam1++;
    if (iThresParam1 < 1)  iThresParam1 = 1;
    MDetector.setThresholdParams(iThresParam1, iThresParam2);
    // recompute
    MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters);
    TheInputImage.copyTo(TheInputImageCopy);
    for (unsigned int i = 0; i < TheMarkers.size(); i++)
        TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255));

    // draw a 3d cube in each marker if there is 3d info
    if (TheCameraParameters.isValid())
        for (unsigned int i = 0; i < TheMarkers.size(); i++)
            CvDrawingUtils::draw3dCube(TheInputImageCopy, TheMarkers[i], TheCameraParameters);

    cv::imshow("in", resize(TheInputImageCopy,1280));
    cv::imshow("thres", resize(MDetector.getThresholdedImage(),1280));
}
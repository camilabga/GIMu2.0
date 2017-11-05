#include "Cow.h"

using namespace std;
using namespace cv;

int velD, velE;

int main(){
    //open webcam
    //VideoCapture capture("arena.mp4");
    VideoCapture capture(1);
    if ( !capture.isOpened() ){
      cout << "Cannot open the video file" << endl;
      return -1;
    }

    /*create Cow -> first initialization has no center
    and the still scans the whole Mat, also, do not contain
    any rectangle center defined */

    Cow cow;
    Mat frame;

    while (1){
        if (!capture.read(frame)) {
            cout<<"\n Cannot read the video file. \n";
            break;
        } 

        cow.setROI(frame);
        cow.transformImage(); 
        cow.searchSquares();
        
        if (cow.find()){
            cow.detectLimits();
            cow.drawCenter(frame);
            if (cow.isCentered()) {
                if (cow.isAlign()) {
                    velE = LOOKING_SPEED;
                    velD = LOOKING_SPEED;
                    // VAI RETO
                } else {
                    // ALINHAR 180 GRAUS COM A VACA
                    if (cow.getSlope() > 100) {
                        
                    } else {
                        
                    }
                }
            } else {
                // GIRAR CONTROLADO
                velE = TURNING_SPEED;
                velD = -TURNING_SPEED;
            }
        } else {
            // GIRAR LOUCAMENTE
            velE = TURNING_SPEED;
            velD = -TURNING_SPEED;
        }

        namedWindow("Original", WINDOW_NORMAL);
        resizeWindow("Original", WIDTH, HEIGHT);
        imshow("Original", frame);

        if (waitKey(1) == 27){
            break;
        }
    }
    
    return 0;
}
#include "Cow.h"

using namespace std;
using namespace cv;

int main(){
    //open webcam
    //VideoCapture capture("arena.mp4");
    VideoCapture capture(0);

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
            cow.distinguishParts(frame);
            cow.drawCenter(frame);
            cow.sendPID(); 
        } else {
            cow.sendPID();
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
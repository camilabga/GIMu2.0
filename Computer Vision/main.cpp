#include "Cow.h"

using namespace std;
using namespace cv;

int main(){
    //open webcam
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
        cow.searchSquares(); // 24/08 SHAME

        if (cow.find()){
            cow.drawCenter(frame);
        } else {
            //send command to look again
        }

        namedWindow("Original", WINDOW_NORMAL);
        resizeWindow("Original", 640, 480);
        imshow("Original", frame);

        if (waitKey(1) == 27){
            break;
        }
    }
    
    return 0;
}
#include "Porteira.h"

using namespace std;
using namespace cv;

int main() {
    Porteira porteira;
    Mat frame;

    VideoCapture capture(0);
    if ( !capture.isOpened() ){
      cout << "Cannot open the video file" << endl;
      return -1;
    }

    while (1){
        if (!capture.read(frame)) {
            cout<<"\n Cannot read the video file. \n";
            break;
        } 

        porteira.setROI(frame);
        porteira.transformImage();
        porteira.setContours();

        namedWindow("Original", WINDOW_NORMAL);
        resizeWindow("Original", WIDTH, HEIGHT);
        imshow("Original", frame);

        if (waitKey(1) == 27){
            break;
        }
    }

    return 0;
}
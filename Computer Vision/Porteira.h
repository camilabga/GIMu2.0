#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#define HEIGHT 480
#define WIDTH 640

class Porteira {
private:
    Point center;
    Point grass;
    Point grabble;

    Mat ROI;
    Mat transformedROI;

public:
    Porteira();

    inline void setROI(const Mat &R){ROI = R.clone();}
    void transformImage();
    void setContours();
    void detectAreas();

};
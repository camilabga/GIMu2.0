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
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

class Cow{
private:
    Point2f center;
    bool detected;
    vector<Point2f> centers;
    Mat ROI;

public:
    Cow();
    Cow(const Cow &C);

    void setROI(const Mat &R);

    void transformImage();
    void searchSquares();

    bool find();
    void drawCenter(Mat &frame);
    
};
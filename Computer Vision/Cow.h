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
#include <queue>

using namespace cv;
using namespace std;

#define HEIGHT 480
#define WIDTH 640

class Cow{
private:
    Point center;
    queue <Point> previous_centers;
    bool detected;
    bool centered;
    vector<vector<Point> > squares;
    Mat ROI;
    Mat transformedROI;

    double angle(Point pt1, Point pt2, Point pt0);

public:
    Cow();

    void setROI(const Mat &R);

    void transformImage();
    void searchSquares();

    bool find();
    bool isCentered();

    void drawCenter(Mat &frame);
    void sendPID();
};
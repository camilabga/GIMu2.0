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
#include <queue>

using namespace cv;
using namespace std;

#define HEIGHT 480
#define WIDTH 640
 
class Cow{
private:
    Point center;
    bool detected;
    bool centered;
    bool aligned;
    float slope;
    vector<vector<Point> > squares;
    vector <Point> centers;
    vector<vector<Point> > legs;
    vector<vector<Point> > body;
    vector<vector<Point>> limits;
    Mat ROI;
    Mat transformedROI;

    double angle(Point pt1, Point pt2, Point pt0);
    double lineInclination(Point pt1, Point pt2);
    void getSlope(Point p1, Point p2, float slope[]);
    void alignSquarePoints();
    
public:
    Cow();

    void setROI(const Mat &R);

    void transformImage();
    void searchSquares();
    void discoverAngle();

    bool find();
    inline bool isCentered(){return centered;}
    inline bool isAlign(){return aligned;}

    void distinguishParts(Mat &R);
    void getInclination(Mat &R);
    void detectLimits();

    void drawCenter(Mat &frame);
    void sendPID();
};
#include "Cow.h"

Cow::Cow(){
    detected = false;
    center.x = 0;
    center.y = 0;
    centers.clear();
}

Cow::Cow(const Cow &C){
    
}

bool Cow::find(){
    if (detected) { // tratamento com ROI e centro anterior
        return true;
    } else { // tratamento com a matriz completa
        return false;
    }
}

void Cow::setROI(const Mat &R){
    if (detected) {

    } else {
        ROI = R.clone();
    }
}

void Cow::transformImage(){
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
    
    erode(ROI, ROI, erodeElement);
    dilate(ROI, ROI, dilateElement);
    
    cvtColor(ROI, ROI, CV_BGR2GRAY);
    blur(ROI, ROI, Size(3,3) );
    
    equalizeHist(ROI, ROI);

    namedWindow("Teste", WINDOW_NORMAL);
    resizeWindow("Teste", 640, 480);
    imshow("Teste", ROI);
}

int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";

static double angle( Point pt1, Point pt2, Point pt0 ){
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void Cow::searchSquares(){
    /*vector<vector<Point>> contours;
    findContours(ROI,contours,RETR_CCOMP,CHAIN_APPROX_SIMPLE);
    vector<Rect> rects;
    
    for (unsigned i=0; i<contours.size(); i++){
        drawContours(ROI,contours,i,Scalar(200,0,0));
        Rect r = boundingRect(contours[i]);
        rects.push_back(r);
    }

    namedWindow("Squares", WINDOW_NORMAL);
    resizeWindow("Squares", 640, 480);
    imshow("Squares", ROI);*/

    Mat canny_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    int thresh = 200;
    
    RNG rng(12345);
    
    Canny(ROI, canny_output, thresh, thresh*2, 3);
    findContours(canny_output, contours, hierarchy, RETR_EXTERNAL ,CHAIN_APPROX_SIMPLE);
    Mat drawing = Mat::zeros(ROI.size(), CV_8UC3 );

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
  
    for(unsigned i = 0; i < contours.size(); i++ ){ 
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }  
  
    /// Draw polygonal contour + bonding rects + circles
    for(unsigned i = 0; i< contours.size(); i++ ){
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
         rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
         circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
    }

    namedWindow( "Contours", WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );

    /*vector<vector<Point> > squares;
    findSquares(squares);
    drawSquares(ROI, squares);*/

}

void Cow::drawCenter(Mat &frame){

}
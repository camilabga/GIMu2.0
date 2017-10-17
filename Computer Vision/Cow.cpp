#include "Cow.h"

int THRESH = 150;
#define max1 500
#define MIN_THRESH 1
#define MAX_THRESH 100
#define MIN_SQUARE_AREA 400

const string trackbarWindowName = "Trackbars";

void on_trackbar( int, void* ){//This function gets called whenever a
	// trackbar position is changed
}   

void createTrackbars(){
	//create window for trackbars
    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "TRHESH", THRESH);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
    //                                  ---->    ---->     ---->
    
    createTrackbar( "THRESH", trackbarWindowName, &THRESH, max1, on_trackbar );
}

Cow::Cow(){
    detected = false;
    centered = false;
    center.x = 0;
    center.y = 0;
    squares.clear();
}

bool Cow::find(){
    center.x = 0;
    center.y = 0;
    int n = 0;
    if (detected) { // tratamento com ROI e centro anterior
        for (size_t i = 0; i < squares.size(); i++){
            for(size_t j = 0; j < squares[i].size(); j++){
                n++;
                center.x = center.x + squares[i][j].x;
                center.y = center.y + squares[i][j].y;
            }
        }

        if (n != 0) {
            center.x = center.x/n;
            center.y = center.y/n;

            if (previous_centers.size() <= 50) {
                previous_centers.push(center);
            } else {
                previous_centers.pop();
                previous_centers.push(center);
            }

            return true;
        } else {
            return false;
        }

    } else {
        return false;
    }
}

void Cow::setROI(const Mat &R){
    if (detected) {
        ROI = R.clone();
    } else {
        ROI = R.clone();
    }
}

void Cow::transformImage(){
    createTrackbars();
    transformedROI = ROI.clone();
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(ROI, ROI, erodeElement);
    dilate(transformedROI, transformedROI, dilateElement);
    
    cvtColor(transformedROI, transformedROI, CV_BGR2GRAY);

    equalizeHist(transformedROI, transformedROI);    

    Mat ROI32;
    transformedROI.convertTo(ROI32, CV_32F);
    threshold( ROI32, ROI32, THRESH, 255, THRESH_BINARY ); 

    float laplacian[]={0,-1,0,
                      -1,4,-1,
                       0,-1,0};

    Mat mask = Mat(3, 3, CV_32F, laplacian);
    filter2D(ROI32, transformedROI, ROI32.depth(), mask, Point(1,1), 0);
    
    transformedROI.convertTo(transformedROI, CV_8UC1);
}

//finds a cosine between 3 points
double Cow::angle( Point pt1, Point pt2, Point pt0 ){
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void Cow::searchSquares(){
    vector<Vec4i> lines;
    vector<Vec4i> lines0;

    squares.clear();

    vector<vector<Point>> contours;
    findContours(transformedROI, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<Point> approx;

    for (size_t i = 0; i < contours.size(); i++){
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if (approx.size() == 4 &&
            fabs(contourArea(Mat(approx))) > MIN_SQUARE_AREA &&
            isContourConvex(Mat(approx))){
                
                double maxCosine = 0;

            for (int j = 2; j < 5; j++){
                // find the maximum cosine of the angle between joint edges
                double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }

            // if cosines of all angles are small
            // (all angles are ~90 degree) then write quandrange
            // vertices to resultant sequence
            if (maxCosine < 0.3) {
                squares.push_back(approx);

                cout << approx[0].x << " " <<  approx[0].y << " ||  "
                     << approx[1].x << " " <<  approx[1].y << " ||  "
                     << approx[2].x << " " <<  approx[2].y << " ||  "
                     << approx[3].x << " " <<  approx[3].y << " " << endl;

            }
        }
    }

    for (size_t i = 0; i < squares.size(); i++){
        const Point *p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(ROI, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
    }

    if (!detected && squares.size() >= 6) {
        detected = true;
    }

    cout << endl;
}

void Cow::drawCenter(Mat &frame){
    circle(frame, center, 5, Scalar(0,255,0), 2, 8, 0 );
    circle(frame, center, 5, Scalar(0,255,0), 2, 8, 0 );
}

void Cow::sendPID(){

    if (detected) {
        if (center.x != 0 && center.y != 0) {
            float erro = center.x - (WIDTH/2);
            erro = erro/(WIDTH/2);
            int to_show;
        
            if (erro < 0.01) { // go left
                to_show = (int)(-100*erro);
                line(ROI,Point((WIDTH/2),center.y),Point(center.x,center.y),Scalar(0,255,0),to_show);
            } else if (erro > 0.01) { // go right
                to_show = (int)(100*erro);
                line(ROI,Point((WIDTH/2),center.y),Point(center.x,center.y),Scalar(0,0,255),to_show);
            }
        }
    }

    namedWindow("PID", WINDOW_NORMAL);
    resizeWindow("PID", WIDTH, HEIGHT);
    imshow("PID", ROI);
}

bool Cow::isCentered(){
    if (!centered) {
        if (previous_centers.size() <= 50) {
            int x = previous_centers.back().x-previous_centers.front().x;
            int y = previous_centers.back().y-previous_centers.front().y;
            if (abs((x*x - y*y)-(center.x * center.x + center.y * center.y)) < 10) {
                centered = true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

bool Cow::isYchanging(){
    if (detected && centered) {
        
    }
}
#include "Cow.h"

int THRESH = 150;
#define max1 500
#define MIN_THRESH 1
#define MAX_THRESH 100
#define MIN_SQUARE_AREA 400
#define FRACTION_CONSIDER_BODY 0.7
#define MAX_CONSIDER_BODY 1.4
#define FRACTION_CONSIDER_LEG 0.3
#define CONSIDER_EQUAL 1
#define PI 3.14159265
#define CONSIDER_CENTERED 5

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
    aligned = false;
    center.x = 0;
    center.y = 0;
    squares.clear();
    centers.clear();
    legs.clear();
    body.clear();
}

bool Cow::find(){
    center.x = 0;
    center.y = 0;
    int n = 0;
    long aux_x = 0, aux_y = 0;

    if (detected) { // tratamento com ROI e centro anterior
        
        alignSquarePoints();
        detectLimits();
        
        for (size_t i = 0; i < squares.size(); i++){
            for(size_t j = 0; j < squares[i].size(); j++){
                n++;
                center.x = center.x + squares[i][j].x;
                center.y = center.y + squares[i][j].y;
                aux_x = aux_x + squares[i][j].x;
                aux_y = aux_y + squares[i][j].y;
            }
            
            aux_x = aux_x/4;
            aux_y = aux_y/4;

            centers.push_back(Point(aux_x, aux_y));

            aux_x = 0;
            aux_y = 0;
        }

        if (n != 0) {
            center.x = center.x/n;
            center.y = center.y/n;

            if (abs(center.x - WIDTH/2) < CONSIDER_CENTERED) {
                centered = true;
            } else {
                centered = false;
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

    //equalizeHist(transformedROI, transformedROI);    

    Mat ROI32;
    transformedROI.convertTo(ROI32, CV_32F);
    threshold( ROI32, ROI32, THRESH, 255, THRESH_BINARY ); 

    float laplacian[]={-1,-1,-1,
                      -1,8,-1,
                       -1,-1,-1};

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

    for (size_t i = 0; i < squares.size(); i++){
        squares[i].clear();
    }
    squares.clear();

    for (size_t i = 0; i < legs.size(); i++){
        legs[i].clear();
    }
    legs.clear();

    for (size_t i = 0; i < body.size(); i++){
        body[i].clear();
    }
    body.clear();

    centers.clear();

    vector<vector<Point>> contours;
    findContours(transformedROI, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
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


                //cout << lineInclination(approx[0], approx[1]) << endl;
                /*cout << approx[0].x << " " <<  approx[0].y << " ||  "
                     << approx[1].x << " " <<  approx[1].y << " ||  "
                     << approx[2].x << " " <<  approx[2].y << " ||  "
                     << approx[3].x << " " <<  approx[3].y << " " << endl;
                */
            }
        }
    }

    for (size_t i = 0; i < squares.size(); i++){
        const Point *p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(ROI, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
    }

    if (!detected && squares.size() >= 4) {
        detected = true;
    }

    //cout << endl;
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

void Cow::distinguishParts(Mat &R){
    /*  LEG // BODY WITH COMPARED SIZES  */
    
    for (size_t i = 0; i < squares.size(); i++){
        if (abs(centers[i].x - squares[i][0].x) / 
            abs(centers[i].y - squares[i][0].y) > FRACTION_CONSIDER_BODY 
            && abs(centers[i].x - squares[i][0].x) / 
            abs(centers[i].y - squares[i][0].y) < MAX_CONSIDER_BODY) {
                body.push_back(squares[i]);
        }

        if (abs(centers[i].x - squares[i][0].x) / 
        abs(centers[i].y - squares[i][0].y) < FRACTION_CONSIDER_LEG) {
            legs.push_back(squares[i]);
        }
    }

    for (size_t i = 0; i < legs.size(); i++){
        const Point *p = &legs[i][0];
        int n = (int)legs[i].size();
        polylines(R, &p, &n, 1, true, Scalar(0, 0, 255), 3, LINE_AA);
    }

    for (size_t i = 0; i < body.size(); i++){
        const Point *p = &body[i][0];
        int n = (int)body[i].size();
        polylines(R, &p, &n, 1, true, Scalar(255, 0, 0), 3, LINE_AA);
    }

    //getInclination(R);

}

double Cow::lineInclination(Point pt1, Point pt2){
    float temp = atan((pt1.y-pt2.y)/(pt1.x-pt2.x + 0.000000001));
    temp = temp*180.0/PI;
    return temp;
}

void Cow::getInclination(Mat &R){
    for (size_t i = 0; i < squares.size(); i++){
        cout << lineInclination(Point(body[i][0].x, body[i][0].y), Point(body[i][1].x, body[i][1].y)) << endl;
    }
}

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int allX[], int allY[], int low, int high){
    int pivot = allX[high];    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (allX[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&allX[i], &allX[j]);
            swap(&allY[i], &allY[j]);
        }
    }
    swap(&allX[i + 1], &allX[high]);
    swap(&allY[i + 1], &allY[high]);
    return (i + 1);
}
 
void quickSort(int allX[], int allY[], int low, int high){
    if (low < high){
        int pi = partition(allX, allY, low, high);
 
        quickSort(allX, allY, low, pi - 1);
        quickSort(allX, allY, pi + 1, high);
    }
}

void Cow::alignSquarePoints(){
    vector<vector<Point> > squaresOK;
    vector <Point> quadrado;
    int allX[4], allY[4]; 
    for (size_t i = 0; i < squares.size(); i++) {
        quadrado.clear();
        allX[0]=squares[i][0].x;
        allX[1]=squares[i][1].x;
        allX[2]=squares[i][2].x;
        allX[3]=squares[i][3].x;

        allY[0]=squares[i][0].y;
        allY[1]=squares[i][1].y;
        allY[2]=squares[i][2].y;
        allY[3]=squares[i][3].y;

        int n = sizeof(allX)/sizeof(allX[0]);
        
        quickSort(allX, allY, 0, n-1);

        if (allY[0] > allY[1]) {
            swap(allX[0], allX[1]);
            swap(allY[0], allY[1]);
        }

        if (allY[3] > allY[2]) {
            swap(allX[3], allX[2]);
            swap(allY[3], allY[2]);
        }

        quadrado.push_back(Point(allX[0], allY[0]));
        quadrado.push_back(Point(allX[1], allY[1]));
        quadrado.push_back(Point(allX[2], allY[2]));
        quadrado.push_back(Point(allX[3], allY[3]));
        squaresOK.push_back(quadrado);
    }

    squares.clear();
    for (size_t i = 0; i < squaresOK.size(); i++) {
        squares.push_back(squaresOK[i]);
    }
    //cout << endl;   
}

void Cow::getSlope(Point p1, Point p2, float slope[]){
    slope[0] = (p1.y-p2.y)/(p1.x-p2.x+0.0000000000000001);
    slope[1] = p1.y - slope[0]*p1.x;
}

void Cow::detectLimits(){
    for (size_t i = 0; i < limits.size(); i++){
        limits[i].clear();
    }
    
    limits.clear();

    vector<Point>aux;

    float slopeUp[2];
    float slopeDown[2];
    float slopeLeft[2];
    float slopeRight[2];
    for (size_t i = 0; i < squares.size(); i++) {
        getSlope(squares[i][3], squares[i][0], slopeUp);
        getSlope(squares[i][0], squares[i][1], slopeLeft);
        getSlope(squares[i][1], squares[i][2], slopeDown);
        getSlope(squares[i][2], squares[i][3], slopeRight);

        for(size_t j = i+1; j < squares.size(); j++){
            if (abs(squares[j][0].x * slopeUp[0] + slopeUp[1] - squares[j][0].y) <= CONSIDER_EQUAL 
             || abs(squares[j][3].x * slopeUp[0] + slopeUp[1] - squares[j][3].y) 
                    <= CONSIDER_EQUAL) {
                        line(ROI, squares[i][0], squares[j][3], Scalar(0, 0, 255), 4, 8, 0);
                        aux.push_back(squares[i][0]);
                        aux.push_back(squares[j][3]);
                        limits.push_back(aux);
            }

            /*if (abs(squares[j][0].x * slopeLeft[0] + slopeLeft[1] - squares[j][0].y) <= CONSIDER_EQUAL 
            || abs(squares[j][1].x * slopeLeft[0] + slopeLeft[1] - squares[j][1].y) 
                   <= CONSIDER_EQUAL) {
                       line(ROI, squares[i][0], squares[j][1], Scalar(0, 0, 255), 4, 8, 0);
                aux.push_back(squares[i][0]);
                aux.push_back(squares[j][1]);
                limits.push_back(aux);
            }

           if (abs(squares[j][2].x * slopeRight[0] + slopeRight[1] - squares[j][2].y) <= CONSIDER_EQUAL 
           || abs(squares[j][3].x * slopeRight[0] + slopeRight[1] - squares[j][3].y) 
                  <= CONSIDER_EQUAL) {
                      line(ROI, squares[i][2], squares[j][3], Scalar(0, 0, 255), 4, 8, 0);
                aux.push_back(squares[i][2]);
                aux.push_back(squares[j][3]);
                limits.push_back(aux);
            }*/
        }
    }

    discoverAngle();

    namedWindow("Limits", WINDOW_NORMAL);
    resizeWindow("Limits", WIDTH, HEIGHT);
    imshow("Limits", ROI);
}

void Cow::discoverAngle(){
    vector<float> angulos;
    float aux = 0;
    if (limits.size() > 0) {
        for (unsigned i = 0; i < limits.size(); i++) {
            aux = (limits[0][0].x - limits[0][1].x) / 
                (sqrt((limits[0][0].x - limits[0][1].x)*(limits[0][0].x - limits[0][1].x)
                    +(limits[0][0].y - limits[0][1].y)*(limits[0][0].y - limits[0][1].y)));
            angulos.push_back((acos(aux)*180/PI));
        }

        slope = angulos[0];
        if (slope == 180 || slope == 0) aligned = true;
    } else {
        // proximo threshold
    }
}
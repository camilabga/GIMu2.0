#include "Cow.h"

int THRESH = 150;
#define max1 500
#define MIN_THRESH 1
#define MAX_THRESH 100
#define SPEED 180
#define BYTES 10

//char buf[BYTES*4];

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

    //TRANSFORMADA DE HOUGH --- ACHAR LINHAS
	/*#if 0
    HoughLines(transformedROI, lines0, 1, CV_PI/180, 100 );

    for( size_t i = 0; i < lines0.size(); i++ )
    {
            float rho = lines0[i][0];
            float theta = lines0[i][1];
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            Point pt1(cvRound(x0 + 1000*(-b)),
                                cvRound(y0 + 1000*(a)));
            Point pt2(cvRound(x0 - 1000*(-b)),
                                cvRound(y0 - 1000*(a)));
            line(ROI, pt1, pt2, Scalar(0,0,255), 3, 8 );
    }
    #else
    // ultimos tres elementos                  (threshold, minLineLength, maxLineGap)
        
        HoughLinesP(transformedROI, lines, 1, CV_PI/180, 50, 20, 30);

        for (unsigned int i = 0; i < lines.size(); i++){
            line(ROI, Point(lines[i][0], lines[i][1]),
                Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
        }

        namedWindow("Teste", WINDOW_NORMAL);
        resizeWindow("Teste", WIDTH, HEIGHT);
        imshow("Teste", ROI);

        for(unsigned int i = 0; i < lines.size(); i++){
            if (abs(angle(Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Point(lines[i+1][0], lines[i+1][1])) < 0.05)){
                points.push_back(Point(lines[i][0], lines[i][1]));
                points.push_back(Point(lines[i][2], lines[i][3]));
                points.push_back(Point(lines[i+1][0], lines[i+1][1]));
            }
            if (abs(angle(Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Point(lines[i+1][2], lines[i+1][3])) < 0.05)){
                points.push_back(Point(lines[i][0], lines[i][1]));
                points.push_back(Point(lines[i][2], lines[i][3]));
                points.push_back(Point(lines[i+1][2], lines[i+1][3]));
            }
        }
    
        for (unsigned int i = 0; i < points.size(); i++){
            circle(ROI, points[i], 5, Scalar(255,0,0), 2, 8, 0 );
        }*/
    
        /*namedWindow("Teste1", WINDOW_NORMAL);
        resizeWindow("Teste1", WIDTH, HEIGHT);
        imshow("Teste1", ROI);*/

    squares.clear();

    //for (unsigned t = MIN_THRESH; t < MAX_THRESH; t++) {
        //threshold(transformedROI, transformedROI, THRESH, 255, THRESH_BINARY );
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
            fabs(contourArea(Mat(approx))) > 600 &&
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
            if (maxCosine < 0.3)
                squares.push_back(approx);
        }
    }

    for (size_t i = 0; i < squares.size(); i++){
        const Point *p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(ROI, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
    }
    //}

    if (!detected && squares.size() >= 6) {
        detected = true;
    }

    /*namedWindow("Teste1", WINDOW_NORMAL);
    resizeWindow("Teste1", WIDTH, HEIGHT);
    imshow("Teste1", ROI);*/

    //#endif

    // #### TESTE 01 ####
    /*vector<vector<Point>> contours;
    findContours(ROI,contours,RETR_CCOMP,CHAIN_APPROX_SIMPLE);
    vector<Rect> rects;
    
    for (unsigned i=0; i<contours.size(); i++){
        drawContours(ROI,contours,i,Scalar(200,0,0));
        Rect r = boundingRect(contours[i]);
        rects.push_back(r);
    }

    namedWindow("Squares", WINDOW_NORMAL);
    resizeWindow("Squares", WIDTH, HEIGHT);
    imshow("Squares", ROI);*/

    // #### TESTE 02 ####

    /*Mat canny_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    int thresh = 200;
    RNG rng(12345);
    
    Canny(ROI, canny_output, thresh, thresh*2, 3);
    findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    cout << contours.size() << endl;

    vector<vector<Point> > contours_poly;
    vector<Rect> boundRect;
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    vector<Point> temp;
  
    for(unsigned i = 0; i < contours.size(); i++ ){ 
        if (contours[i].size() == 4) {
            approxPolyDP( Mat(contours[i]), temp, 3, true );
            contours_poly.push_back(temp);
            boundRect.push_back(boundingRect( Mat(temp) ));
            minEnclosingCircle( (Mat)temp, center[i], radius[i] );
        }
    }
  
    /// Draw polygonal contour + bonding rects + circles
    for(unsigned i = 0; i< contours.size(); i++ ){
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         drawContours(ROI, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
         rectangle(ROI, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
         circle(ROI, center[i], (int)radius[i], color, 2, 8, 0 );
    }

    namedWindow( "Contours", WINDOW_AUTOSIZE );
    imshow( "Contours", ROI);*/

    /*vector<vector<Point> > squares;
    findSquares(squares);
    drawSquares(ROI, squares);*/




}

void Cow::drawCenter(Mat &frame){
    circle(frame, center, 5, Scalar(0,255,0), 2, 8, 0 );
    circle(frame, center, 5, Scalar(0,255,0), 2, 8, 0 );
}

void Cow::conectI2C(bool ok, int velE, int velD){
    for(int i=0;i<BYTES*4;i++){
        buf[i] = '\0';
    }
    char cmd[BYTES+1];

    cmd[0] = (char) ok;
    cmd[1] = (char) velE;
    cmd[2] = (char) velD;
    cmd[3] = ';';

    //Enviar comando:
    arduino.i2cWrite(cmd, BYTES);
    usleep(10000);

    //Receber resposta:
    if(arduino.i2cRead(buf, BYTES) == BYTES){
        buf[(BYTES*4)-1] = '\0';
        cout << "Retorno: " << buf << endl;
    }else{
        cout << "Erro !" << endl;
    }
}

void Cow::sendSerial(float erro, unsigned i){
    int to_send = (int)(erro*100+100);
    char to_send_char [3];
    switch(i){
        case 1:
        to_send_char = {'r', (char)to_send, ';'};
        break;

        case 2:
        to_send_char = {'f', '0', ';'};
        break;

        case 3:
        to_send_char = {'p', '0', ';'};
        break;

    }
    

    FILE * pFile;

    pFile = fopen ("/dev/ttyUSB0", "w");

    cout<< to_send_char <<endl;

    fputs(to_send_char, pFile);
    fclose(pFile);
}

void Cow::sendPID(){
    if (detected) {
        if (center.x != 0 && center.y != 0) {
            float erro = center.x - (WIDTH/2);
            erro = erro/(WIDTH/2);
            int to_send, to_show;
        
            if (erro < 0.01) { // go left
                to_show = (int)(-100*erro);
                line(ROI,Point((WIDTH/2),center.y),Point(center.x,center.y),Scalar(0,255,0),to_show);
                //conectI2C(1, -SPEED*erro, SPEED);
                
            } else if (erro > 0.01) { // go right
                to_show = (int)(100*erro);
                line(ROI,Point((WIDTH/2),center.y),Point(center.x,center.y),Scalar(0,0,255),to_show);
                //conectI2C(1, SPEED, SPEED*erro);
            }

            sendSerial(erro, 1);

        } else {
            sendSerial(0, 3);         
        }

    } else {
        //conectI2C(2,0,0);
        sendSerial(0,2);
    }

    namedWindow("PID", WINDOW_NORMAL);
    resizeWindow("PID", WIDTH, HEIGHT);
    imshow("PID", ROI);
}
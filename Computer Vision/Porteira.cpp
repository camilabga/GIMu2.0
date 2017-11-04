#include "Porteira.h"

#define THRESH 150

Porteira::Porteira(){
    center.x = 0;
    center.y = 0;

    grass.x = 0;
    grass.y = 0;

    grabble.x = 0;
    grabble.y = 0;
}

void Porteira::transformImage(){
    transformedROI = ROI.clone();
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(ROI, ROI, erodeElement);
    dilate(transformedROI, transformedROI, dilateElement);

    GaussianBlur(transformedROI, transformedROI, Size(3, 3), 0, 0, BORDER_DEFAULT);

    namedWindow("Transformed", WINDOW_NORMAL);
    resizeWindow("Transformed", WIDTH, HEIGHT);
    imshow("Transformed", transformedROI);
}

void Porteira::setContours(){
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

    namedWindow("Contornos", WINDOW_NORMAL);
    resizeWindow("Contornos", WIDTH, HEIGHT);
    imshow("Contornos", transformedROI);
}

void detectAreas(){
}
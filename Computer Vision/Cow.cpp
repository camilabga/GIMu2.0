#include "Cow.h"

Cow::Cow(){
    detected = false;

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

    equalizeHist(ROI, ROI);

    namedWindow("Teste", WINDOW_NORMAL);
    resizeWindow("Teste", 640, 480);
    imshow("Teste", ROI);
}

void Cow::searchSquares(){
    
}

void Cow::drawCenter(Mat &frame){

}
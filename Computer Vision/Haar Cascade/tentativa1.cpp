//* Para compilar 
//$  g++ -o obj_rec obj_rec.cpp -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_legacy
//
//*/

#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;


int main(int argc, const char *argv[]) {
    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if (argc != 3) {
        cout << "usage: " << argv[0] << " </path/to/haar_cascade> </path/to/file>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        cout << "\t </path/to/file> -- Path to file for detect the object." << endl;
        exit(1);
    }
    // Get the path to your CSV:
    string fn_haar = string(argv[1]);
    string fn_image = string(argv[2]);
    // These vectors hold the images and corresponding labels:
    Mat image;
    // Read image file
    try {
        image = imread(fn_image);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_image << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }

    

    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
	 while (1) {
         
        // Clone the current frame:
        Mat original = image.clone();
        // Convert the current frame to grayscale:
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        // Find the faces in the frame:
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);
        // At this point you have the position of the faces in
        // faces. Now we'll get the faces, make a prediction and
        // annotate it in the video. Cool or what?
        
        // sstream scont = "";

        for(unsigned i = 0; i < faces.size(); i++) {
            // Process face by face:
            Rect face_i = faces[i];
            // Crop the face from the image. So simple with OpenCV C++:
            stringstream ss;
            ss << "neg" << i+23 << ".jpg";

            Mat save(original, face_i);

            imwrite(ss.str(),save); //save ROI image

            cout << ss.str() << endl;
            Mat face = gray(face_i);

            // And finally write all we've found out to the original image!
            // First of all draw a green rectangle around the detected face:
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            // Create the text we will annotate the box with:
            string box_text = " Objeto ";
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            // And now put it into the image:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
        }
        // Show the result:
        imshow("face_recognizer", original);
        // And display it:
        char key = (char) waitKey(20);
        // Exit this loop on escape:
        if(key == 27)
            break;
	}
    
    return 0;
}

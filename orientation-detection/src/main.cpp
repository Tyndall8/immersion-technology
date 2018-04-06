// For Immersion Technology


#include <sstream>
#include <string>
#include <iostream>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include "CameraAngle.h"

using namespace cv;
using namespace std;

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string window_bestAngle = "Best Angle";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";


void on_trackbar( int, void* )
{//This function gets called whenever a
    // trackbar position is changed


}
void createTrackbars()
{
    //create window for trackbars


    namedWindow(trackbarWindowName,0);
    //create memory to store trackbar name on window
    char TrackbarName[50];
    sprintf( TrackbarName, "H_MIN", H_MIN);
    sprintf( TrackbarName, "H_MAX", H_MAX);
    sprintf( TrackbarName, "S_MIN", S_MIN);
    sprintf( TrackbarName, "S_MAX", S_MAX);
    sprintf( TrackbarName, "V_MIN", V_MIN);
    sprintf( TrackbarName, "V_MAX", V_MAX);
    //create trackbars and insert them into window
    //3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
    //the max value the trackbar can move (eg. H_HIGH),
    //and the function that is called whenever the trackbar is moved(eg. on_trackbar)
    //                                  ---->    ---->     ---->
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}

int main(int argc, char* argv[])
{
    // Control flags
    bool showBestAngle = true;


    createTrackbars();

    CameraAngle angle0(0);
    CameraAngle angle1(1);

    vector<CameraAngle> angles;
    angles.push_back(angle0);
    angles.push_back(angle1);

    for (int i = 0; i < angles.size(); i++) {
        angles[i].startCapture();
    }

    while (true) {

        for (int i = 0; i < angles.size(); i++) {
            angles[i].read();
            angles[i].convertColor2HSV();
            // can choose whether or not to do morphOps here
            //angles[i].filterHSV(H_MIN, S_MIN, V_MIN, H_MAX, S_MAX, V_MAX);
            angles[i].filterHSV(0, 106, 63, 98, 236, 188);
        }

        double largestArea = 0;
        int bestAngle_ID = 0;
        for (int i = 0; i < angles.size(); i++) {
            angles[i].trackFilteredObject();
            if (angles[i].getConfidence() > largestArea) {
                largestArea = angles[i].getConfidence();
                bestAngle_ID = angles[i].getID();
            }
        }

        if (angles[bestAngle_ID].getObjectStatus()) {
            angles[bestAngle_ID].drawText();
            angles[bestAngle_ID].drawObject();
            imshow(window_bestAngle, angles[bestAngle_ID].getColorImage());
        }

        for (int i = 0; i < angles.size(); i++) {
            angles[i].showThreshold();
        }


        if (waitKey(30) == 27) {
            break;
        }
    }
}
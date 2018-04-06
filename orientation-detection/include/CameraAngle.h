//
// Created by reid on 2/27/18.
//

#ifndef ORIENTATION_DETECTION_CAMERAANGLE_H
#define ORIENTATION_DETECTION_CAMERAANGLE_H

#define pi 3.14159265359

#include <iostream>
#include <cmath>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "utils.h"

using namespace std;
using namespace cv;

class CameraAngle {

public:

    // default frame dimensions
    const int FRAME_WIDTH = 640;
    const int FRAME_HEIGHT = 480;
    // maximum number of tracked objects
    const int MAX_NUM_OBJECTS = 50;
    // min/max area of object
    const int MIN_OBJECT_AREA = 20 * 20;
    const int MAX_OBJECT_AREA = FRAME_WIDTH*FRAME_HEIGHT / 1.5;
    // window names
    const string window_color = "[" + getIDString() + "] - Color Image";
    const string window_HSV = "[" + getIDString() + "] - HSV Image";
    const string window_threshold = "[" + getIDString() + "] - Threshold Image";


    // Constructors
    CameraAngle(int cameraID) {
        ID = cameraID;
    }

    // starts the VideoCapture
    bool startCapture();
    // read frame
    bool read();
    // convert from color space to HSV
    void convertColor2HSV();
    // generate binary image from HSV
    void filterHSV(int H_MIN, int S_MIN, int V_MIN, int H_MAX, int S_MAX, int V_MAX,
                    bool doMorphOps=true);
    // track object and find area
    void trackFilteredObject();
    // draw text on color image
    void drawText(bool showConfidence=true);
    // draw tracker on color image
    void drawObject(Scalar color=Scalar(255, 0, 0));


    // get int value of ID
    int getID() {
        return ID;
    }
    // get string value of ID
    string getIDString() {
        return intToString(ID);
    }
    // get size of tracked object
    double getConfidence() {
        return objectArea;
    }
    bool getObjectStatus() {
        return objectFound;
    }
    // get color image matrix
    Mat getColorImage() {
        return colorImage.clone();
    }

    void showColorImage() {
        imshow(window_color, colorImage);
    }
    void showThreshold() {
        imshow(window_threshold, binaryImage);
    }

private:

    // ID for camera
    int ID;
    // video capture object
    VideoCapture capture;
    // camera feed matrix
    Mat colorImage;
    // HSV matrix
    Mat HSVImage;
    // binary threshold matrix
    Mat binaryImage;
    // coordinates for tracked object
    int x, y;
    // current size of the tracked object
    double objectArea;
    // current diameter of tracked object
    double objectDiameter;
    // distance from camera to tracked object
    double objectDistance;
    double cmDiameter;
    // bool to keep track of whether there is an object being tracked
    bool objectFound;
    double focalLength;
    double knownWidth = 0.075;
    double knownDistance = 0.3048;

};


#endif //ORIENTATION_DETECTION_CAMERAANGLE_H

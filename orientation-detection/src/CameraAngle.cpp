//
// Created by reid on 2/27/18.
//

#include "CameraAngle.h"

bool CameraAngle::startCapture()
{
    capture.open(ID);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
}

bool CameraAngle::read()
{
    return capture.read(colorImage);
}

void CameraAngle::convertColor2HSV()
{
    cvtColor(colorImage, HSVImage, CV_BGR2HSV);
}

void CameraAngle::filterHSV(int H_MIN, int S_MIN, int V_MIN, int H_MAX, int S_MAX, int V_MAX,
                            bool doMorphOps)
{
    // filter HSV image between the value ranges, store filtered image in binary
    inRange(HSVImage, Scalar(H_MIN,S_MIN,V_MIN), Scalar(H_MAX,S_MAX,V_MAX), binaryImage);

    if (doMorphOps) {
        // create structuring element that will be used to "dilate" and "erode" image.
        // the element chosen here is a 3px by 3px rectangle
        Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
        // dilate with larger element so make sure object is nicely visible
        Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

        erode(binaryImage, binaryImage, erodeElement);
        erode(binaryImage, binaryImage, erodeElement);
        dilate(binaryImage, binaryImage, dilateElement);
        dilate(binaryImage, binaryImage, dilateElement);
    }
}

void CameraAngle::trackFilteredObject()
{
    // operate on a copy instead of original
    Mat tempBinary;
    binaryImage.copyTo(tempBinary);

    // contours / hierarchy
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // find contours of filtered image
    findContours(tempBinary, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    int numObjects = hierarchy.size();
    if (numObjects > 0 && numObjects < MAX_NUM_OBJECTS) {
        // keep track of biggest object within range
        double maxArea = 0;
        objectArea = 0;
        objectFound = false;
        // loop through found objects
        for (int i = 0; i >= 0; i = hierarchy[i][0]) {
            // use moments to compare found objects
            Moments moment = moments((cv::Mat) contours[i]);
            double areaFound = moment.m00;

            // throw out objects that are too small/too huge
            // save largest area within range, that's the object we want
            if (areaFound>MIN_OBJECT_AREA && areaFound<MAX_OBJECT_AREA && areaFound > maxArea) {
                x = moment.m10 / areaFound;
                y = moment.m01 / areaFound;
                maxArea = areaFound;
                objectFound = true;
            }
        }
        // store final value in member variable
        objectArea = maxArea;
        // find the diameter
        objectDiameter = 2 * sqrt(objectArea / pi);

        cmDiameter = objectDiameter/3779.52756;


        //TODO: use this value to set the focal length for the application
        //focalLength = (knownDistance * cmDiameter) / knownWidth;
        focalLength = .192;

        objectDistance = knownWidth * focalLength / cmDiameter;
        cout << "object Distance = " << objectDistance << endl;
        cout << "focal length " << focalLength << endl;
        cout << endl << endl;

    }
    // Filter is too noisy
    else if (numObjects > MAX_NUM_OBJECTS) {
        cout << "[ " << ID << " ] - Too much noise! Recommend adjusting the filter" << endl;
    }
    // No objects found
    else {
    }
}

void CameraAngle::drawText(bool showConfidence)
{
    string confidence = "Confidence: ";
    confidence += intToString(objectArea);
    putText(colorImage, "Tracking Object", Point(0, 50), 2, 1, Scalar(255, 0, 0), 2);
    putText(colorImage, confidence, Point(0, 100), 2, 1, Scalar(255, 0, 0), 2);
    putText(colorImage, "Distance: " + to_string(objectDistance)  + " m", Point(0, 150), 2, 1, Scalar(255, 0, 0), 2);
}

void CameraAngle::drawObject(Scalar color)
{
    circle(colorImage,Point(x,y),20,Scalar(0,255,0),2);

    if(y-25>0)
        line(colorImage,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(colorImage,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
        line(colorImage,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(colorImage,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
        line(colorImage,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(colorImage,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
        line(colorImage,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(colorImage,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

    putText(colorImage,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);
}






//void CameraAngle::find_marker(Mat image) {
//    Mat gray, edged;
//    cv::cvtColor(image, gray, CV_BGR2GRAY);
//    cv::GaussianBlur(gray, gray, Size(5, 5), 0);
//    cv::Canny(gray, edged, 35, 125);
//
//    vector< vector<Point> > contours;
//    vector<Vec4i> hierarchy;
//    cv::findContours(edged, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));
//    int
//}


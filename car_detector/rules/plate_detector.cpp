/*
 * plate_detector.cpp
 *
 *  Created on: 2014年11月20日
 *      Author: qin
 */
#include "plate_detector.hpp"


PlateDetector::PlateDetector(){

}

void PlateDetector::detector(Mat& img, Mat& blured, Mat& gray, Mat& hsv, Mat& canny,
                vector<KeyPoint> &ret) {
        Mat mask;
        inRange(hsv, Scalar(100, 40, 0), Scalar(140, 255, 255), mask);
        imshow("Mask", mask);
        blob_detector->detect(mask, ret);
}

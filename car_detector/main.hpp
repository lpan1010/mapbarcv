/*
 * main.hpp
 *
 *  Created on: 2014年11月19日
 *      Author: qin
 */

#ifndef CAR_DETECTOR_MAIN_HPP_
#define CAR_DETECTOR_MAIN_HPP_

#include <utility>
#include <cmath>

#include "stdio.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;


float calc_distance_thread(float x, float row, float col);
Mat CannyThreshold(Mat &gray);


#endif /* CAR_DETECTOR_MAIN_HPP_ */

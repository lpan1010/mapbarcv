/*
 * plate_detector.hpp
 *
 *  Created on: 2014年11月20日
 *      Author: qin
 */

#ifndef PLATE_DETECTOR_HPP_
#define PLATE_DETECTOR_HPP_

#include "tail_light_detector.hpp"
class PlateDetector: public TailLightDetector{
        public:
                PlateDetector();
                void detector(Mat& img, Mat& blured, Mat& gray, Mat& hsv,
                                Mat& canny,
                                vector<KeyPoint> &ret);
};

#endif /* PLATE_DETECTOR_HPP_ */

/*
 * tl_detector_night.hpp
 *
 *  Created on: 2014年11月19日
 *      Author: qin
 */

#include "tail_light_detector.hpp"
class TLDetectorNight: public TailLightDetector {
        public:
                void detector(Mat& img, Mat& blured, Mat& gray, Mat& hsv,
                                Mat& canny,
                                list<pair<KeyPoint, KeyPoint> > &ret) {
                        Mat mask;
                        // 获得过滤红色的mask
                        inRange(gray, Scalar(250), Scalar(255),
                                        mask);
                        imshow("Mask", mask);
                        // 探测红点
                        vector<KeyPoint> red_points;
                        blob_detector->detect(mask, red_points);

                        // 寻找符合条件的红点对
                        list<pair<KeyPoint, KeyPoint> > pairs;
                        find_pairs(pairs, red_points);
                        filter_out_pairs(pairs, canny, ret);
                }
};


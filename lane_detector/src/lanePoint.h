#ifndef LANEPOINT_H
#define LANEPOINT_H
#include "opencv2/core/core.hpp"
class laneLine {
public:
	laneLine(const cv::Point pp1, const cv::Point pp2) :
			p1(pp1), p2(pp2) {
		slope = p1.x - p2.x == 0 ? 10000 : (p2.y - p1.y) / (p2.x - p1.x);
	}
	cv::Point p1;
	cv::Point p2;
	double slope;
};

#endif

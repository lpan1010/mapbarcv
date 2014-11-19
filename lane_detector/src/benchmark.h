#ifndef RESULTCOMPARE_H
#define RESULTCOMPARE_H
#include <iostream>
#include <list>
#include <map>
#include "lanePoint.h"

#include "opencv2/core/core.hpp"

using namespace std;
struct LineSlice {
	cv::Point p1;
	cv::Point p2;
};
class benchmark {
public:
	map<string, list<LineSlice> > benchmarks; //image_name,laneline
//	map<string, string > benchmarks; //image_name,laneline
	void loadBenchmark(const string file);
	double evaluate(const string& image_name, const cv::Point p1, const cv::Point p2);
	double getSlope(double x1, double y1, double x2, double y2);
	double getDistance(const cv::Point p, const cv::Point p1, const cv::Point p2);
	double getDistance(double x, double y, double x1, double y1, double x2, double y2);
	vector<string> split(const string &s, char delim, vector<string> &elems);
};

#endif


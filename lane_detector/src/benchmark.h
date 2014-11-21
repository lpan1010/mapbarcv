/*
 * benchmark.h
 *
 *  Created on: 2014年11月18日
 *      Author: kangyong
 */
#ifndef RESULTCOMPARE_H
#define RESULTCOMPARE_H
#include <iostream>
#include <list>
#include <map>
#include "lanePoint.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
struct LineSlice { //Two points fix a line.
	cv::Point p1;
	cv::Point p2;
};
/***
 * Class for lane benchmarks.
 */
class benchmark {
private:
	benchmark() { //Constructor is private.
	}

	//Declare these two function. Make sure they are unaccessable
	//otherwise it may accidently get copies of singleton appearing.
	benchmark(benchmark const &);
	void operator=(benchmark const &);
//	map<string, string > benchmarks; //image_name,laneline
public:
	static map<string, list<LineSlice> > benchmarks;	//label data collection
	/**
	 * get singleton
	 */
	static benchmark& getInstance() {
		static benchmark instance;
		return instance;
	}
	/**
	 * load label data
	 */
	void loadBenchmark(const string& file);
	/**
	 * evaluate the recognition line(point1,point2)
	 * @param image_name label image name
	 * @param p1 one point of line
	 * @param p2 one point of line
	 * @param img the image of Mat format. The purpose of this parameter is to print lines of labels.
	 */
	double evaluate(const string& image_name, const cv::Point& p1, const cv::Point& p2, cv::Mat& img);
	/**
	 * evaluate the recognition line(point1,point2)
	 * @param image_name label image name
	 * @param p1 one point of line
	 * @param p2 one point of line
	 */
	double evaluate(const string& image_name, const cv::Point& p1, const cv::Point& p2);
	/**
	 *get slope of these points fixed line.
	 */
	double getSlope(double x1, double y1, double x2, double y2);
	/**
	 *get the distance from point p to line (p1,p2).
	 */
	double getDistance(const cv::Point& p, const cv::Point& p1, const cv::Point& p2);
	/**
	 *get the distance from point(x,y) to line ((x1,y1),(x2,y2)).
	 *@return the distance of point(x,y) to line
	 */
	double getDistance(double x, double y, double x1, double y1, double x2, double y2);
	/**
	 *get minimum distance from point p1 and p2 to LineSlice collector.
	 *@param p1,p2 these two points fixed a line .
	 *@param listSlice line collection.
	 *@return the minimum distance from point p1 and p2 to one line of line collector.
	 */
	double getMinDistance(const cv::Point& p1, const cv::Point& p2, list<LineSlice>& listSlice);
	/**
	 *split string s by delim.
	 *@return elements of the token.
	 */
	vector<string> split(const string &s, char delim, vector<string> &elems);
};

#endif


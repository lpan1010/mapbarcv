/*
 * benchmark.cpp
 *
 *  Created on: 2014年11月18日
 *      Author: kangyong
 */
#include "benchmark.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;
using namespace cv;

vector<string> benchmark::split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
template<typename T>
vector<T> splits(const string &s, char delim, vector<T> &elems) {
	stringstream ss(s);
	string item;
	T t;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
//initialized (must)
map<string, list<LineSlice> > benchmark::benchmarks;
void benchmark::loadBenchmark(const string& file_name) {
	ifstream infile(file_name.c_str());
	string line;
	string key, value;
	while (getline(infile, line)) {
		int offset = line.find(" ");
		key = line.substr(0, offset);
		list<LineSlice> value;
		Point p;
		LineSlice l;
		istringstream iss(line.substr(offset, line.length()));
		do {
			string slice;
			iss >> slice;
			if (slice.empty()) {
				continue;
			}
			vector<string> points;
			split(slice, ',', points);
			if (points.size() == 4) {
				stringstream(points[0]) >> p.x;
				stringstream(points[1]) >> p.y;
				l.p1 = p;
				stringstream(points[2]) >> p.x;
				stringstream(points[3]) >> p.y;
				l.p2 = p;
				value.push_back(l);
			}
		} while (iss);
		benchmark::benchmarks[key] = value;
	}
	cout << "load benchmarks finished!  Size = " << benchmarks.size() << endl << endl;
}
double benchmark::evaluate(const string& image_name, const cv::Point& p1, const cv::Point& p2) {
	Mat img;
	return evaluate(image_name, p1, p2, img);
}
double benchmark::evaluate(const string& image_name, const cv::Point& p1, const cv::Point& p2, cv::Mat& img) {
	list<LineSlice> testlist = benchmarks[image_name];
//	if (img.empty()) {
	for (list<LineSlice>::iterator it = testlist.begin(); it != testlist.end(); ++it) {
		LineSlice l = *it;
		line(img, l.p1, l.p2, Scalar(0, 0, 255), 1, 16);
	}
//	}
	double minDistance = getMinDistance(p1, p2, testlist);
	cout << "min distance = " << minDistance << endl;
	return 0.0;
}
double benchmark::getMinDistance(const cv::Point& p1, const cv::Point& p2, list<LineSlice>& listSlice) {
	double minDistance = 5000;
	for (list<LineSlice>::iterator it = listSlice.begin(); it != listSlice.end(); ++it) {
		LineSlice l = *it;
		double dis = getDistance(l.p1, p1, p2);
		double dis2 = getDistance(l.p2, p1, p2);
//		cout<<"current min distance = "<<minDistance<<endl;
		if (dis + dis2 < minDistance) {
			minDistance = dis + dis2;
		}
	}
	return minDistance;
}
double benchmark::getSlope(double x1, double y1, double x2, double y2) {
	return x2 - x1 == 0 ? 100000 : (y2 - y1) / (x2 - x1); //if slope not exist ,return what ?(1000)
}
double benchmark::getDistance(const Point& p, const Point& p1, const Point& p2) {
	return getDistance(p.x, p.y, p1.x, p1.y, p2.x, p2.y);
}
double benchmark::getDistance(double x, double y, double x1, double y1, double x2, double y2) {
	double A = y1 - y2;
	double B = x2 - x1;
	double C = x1 * y2 - x2 * y1;
	return abs((A * x + B * y + C) / sqrt(A * A + B * B));
}

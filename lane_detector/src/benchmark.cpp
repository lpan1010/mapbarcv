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
template <typename T>
vector<T> splits(const string &s, char delim, vector<T> &elems) {
	stringstream ss(s);
	string item;
	T t;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
void benchmark::loadBenchmark(const string file_name) {
	ifstream infile(file_name.c_str());
	string line;
	string key, value;
	while (getline(infile, line)) {
		int offset = line.find(" ");
		key = line.substr(0, offset);
		list<LineSlice> value;
		Point p;
		istringstream iss(line.substr(offset, line.length()));
		do {
			string slice;
			iss >> slice;
			if (slice.empty()) {
				continue;
			}
			cout << "slice line  = " << slice << endl;
			vector<string> points;
			split(slice,',',points);
			for(int i =0;i<points.size();i++){
				cout<<points[i]<<endl;
			}
		} while (iss);
//		cout << "key =" << key << " value=" << value << endl;
//		benchmark::benchmarks[key] = value;
	}
}
double benchmark::evaluate(const string& image_name, const cv::Point p1, const cv::Point p2) {
//	string value = benchmarks[image_name];
	string value = "232,2323";
	istringstream iss(value);
	list<double> points;
	do {
		string slice;
		iss >> slice;
		if (slice.empty()) {
			continue;
		}
		cout << "slice line  = " << slice << endl;
	} while (iss);
	double slope = getSlope(p1.x, p1.y, p2.x, p2.y);
	cout << "slope = " << slope << endl;
	//		istringstream iss(line);
	return 0.0;
}
double benchmark::getSlope(double x1, double y1, double x2, double y2) {
	return x2 - x1 == 0 ? 100000 : (y2 - y1) / (x2 - x1); //if slope not exist ,return what ?(1000)
}
double benchmark::getDistance(const Point p, const Point p1, const Point p2) {
	return getDistance(p.x, p.y, p1.x, p1.y, p2.x, p2.y);
}
/**
 * (x,y) 到 (x1,y1)(x2,y2)确定的直线的距离
 */
double benchmark::getDistance(double x, double y, double x1, double y1, double x2, double y2) {
	double A = y1 - y2;
	double B = x2 - x1;
	double C = x1 * y2 - x2 * y1;
	return (A * x + B * y + C) / sqrt(A * A + B * B);
}

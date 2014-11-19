#include "laneDetection.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include<algorithm>

#include <iostream>
#include<vector>
#include<list>
using namespace std;
using namespace cv;

template<typename T>
std::string to_string(T const & value) {
	stringstream sstr;
	sstr << value;
	return sstr.str();
}
const size_t queue_container_number = 5;
void testLane1(Mat& inputImage, Mat& outputImage, int laneLen = 1) {
	//test
	outputImage.create(inputImage.size(), inputImage.type());
	outputImage.setTo(0);
	int aux = 0;
	for (int j = 0; j < inputImage.cols; j++) {
		unsigned char *ptrRowSrc = inputImage.ptr<uchar>(j);
		unsigned char *ptrRowDst = outputImage.ptr<uchar>(j);
		for (int i = laneLen; i < inputImage.rows - laneLen; ++i) {
			aux = 2 * ptrRowSrc[i];
			aux += -ptrRowSrc[i - laneLen];
			aux += -ptrRowSrc[i + laneLen];
			aux += -abs((int) ptrRowSrc[i - laneLen] - ptrRowSrc[i + laneLen]);

			aux = aux < 0 ? 0 : aux;
			aux = aux > 255 ? 255 : aux;
			ptrRowDst[i] = (unsigned char) aux;
		}
	}
}
void LaneDetection::markLaneDetection(Mat& inputImage, Mat& outputImage, int laneLen = 1) {
	outputImage.create(inputImage.size(), inputImage.type());
	outputImage.setTo(0);
	int aux = 0;
	for (int j = 0; j < inputImage.rows; j++) {
		unsigned char *ptrRowSrc = inputImage.ptr<uchar>(j);
		unsigned char *ptrRowDst = outputImage.ptr<uchar>(j);
		for (int i = laneLen; i < inputImage.cols - laneLen; ++i) {
			aux = 2 * ptrRowSrc[i];
			aux += -ptrRowSrc[i - laneLen];
			aux += -ptrRowSrc[i + laneLen];
			aux += -abs((int) ptrRowSrc[i - laneLen] - ptrRowSrc[i + laneLen]);

			aux = aux < 0 ? 0 : aux;
			aux = aux > 255 ? 255 : aux;
			ptrRowDst[i] = (unsigned char) aux;
		}
	}

}
bool r_w = false;
bool l_w = false;

//std::queue<vector<Vec2f>> q_left;
list<double> list_left;
list<double> list_right;
//std::queue<int> q;
size_t cou = 0;
int LaneDetection::laneDetection(Mat& inputImage, Mat& inputImage_gray, LogCPlus& log) {
	if (!inputImage.data) {
		return -1;
	}
//	laneResult* rCompare = laneResult::getInstance();

//	cout<<inputImage.size()<<endl;
	if (inputImage.channels() == 3)
		cvtColor(inputImage, inputImage_gray, CV_BGR2GRAY);
	else
		inputImage.copyTo(inputImage_gray);
	imshow("inputImage_gray", inputImage_gray);
	//	blur(src_gray, detected_edges, Size(1, 1));
	markLaneDetection(inputImage_gray, detected_edges, inputImage.cols / 50); //12
	imshow("laneMarking", detected_edges);
	int lowThreshold = 66;

//	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * 3, kernel_size);
//	namedWindow("Canny",CV_WINDOW_AUTOSIZE);
	imshow("Canny", detected_edges);
	vector<Vec2f> lines;
	HoughLines(detected_edges, lines, 1, CV_PI / 180, inputImage.cols / 12, 0, 0);

	//test
	cout << "lines number: " << lines.size() << endl;
	vector<double> r_vector;
	vector<double> l_vector;
	map<double, double> my_map;
	cou++;
	string debugfileName = to_string(cou);
	for (size_t i = 0; i < lines.size(); i++) {
		float rho = lines[i][0], theta = lines[i][1];
		//	if(theta>1.4&&theta<1.85){//internet
		if (theta > 1.3 && theta < 1.9) {		//1.34 1.799 filter lines
			continue;
		}
		if (rho > 0) {
			l_vector.push_back(theta);
		} else {
			r_vector.push_back(theta);
		}
		my_map[theta] = rho;
		drawLane(inputImage, rho, theta);
	}
	sort(l_vector.begin(), l_vector.end());
	sort(r_vector.begin(), r_vector.end());

	if (l_vector.size() > 0) {
		double theta = l_vector[l_vector.size() / 2];
		double rho = my_map[l_vector[l_vector.size() / 2]];
		if (l_vector.size() > 5) {
			if (list_left.size() < queue_container_number) {
				list_left.push_back(rho);
			} else {
//				list_left.pop_front();
//				list_left.push_back(rho);
			}
		}
		debugfileName += "l_" + to_string(l_vector.size()) + "_" + to_string(rho) + "_" + to_string(theta);
		if (l_vector.size() <= 3 && list_left.size() == queue_container_number && r_vector.size() != 0) {
			double sum = 0;
			for (list<double>::const_iterator ci = list_left.begin(); ci != list_left.end(); ++ci) {
				sum += *ci;
			}
			double mean = sum / queue_container_number;
			if (abs(mean - rho) > 70) {		//if difference is too big with last step, using last step point.
				line(inputImage, l_pt1, l_pt2, Scalar(0, 255, 0), 3, CV_AA);
				//compare
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));
				line(inputImage, pt1, pt2, Scalar(0, 255, 255), 3, CV_AA);
				//end compare
			} else {
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));
				line(inputImage, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
				l_pt1 = pt1;
				l_pt2 = pt2;
				max_left_step = 0;
				list_left.pop_front();
				list_left.push_back(rho);
			}
		} else {
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 5000 * (-b));
			pt1.y = cvRound(y0 + 5000 * (a));
			pt2.x = cvRound(x0 - 5000 * (-b));
			pt2.y = cvRound(y0 - 5000 * (a));
			cout<<pt1.x<<","<<pt1.y<<","<<pt2.x<<","<<pt2.y<<endl;
			line(inputImage, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
			l_pt1 = pt1;
			l_pt2 = pt2;
			max_left_step = 0;
			if (list_left.size() > 0)
				list_left.pop_front();
			list_left.push_back(rho);
		}
	} else {
		if (max_left_step < max_step)
			line(inputImage, l_pt1, l_pt2, Scalar(0, 255, 0), 3, CV_AA);
		max_left_step++;
	}

	if (r_vector.size() > 0) {
		double theta = r_vector[r_vector.size() / 2];
		double rho = my_map[r_vector[r_vector.size() / 2]];
		if (r_vector.size() > 5) {
				if (list_right.size() < queue_container_number) {
					list_right.push_back(rho);
				} else {
					list_right.pop_front();
					list_right.push_back(rho);
				}
			}
		debugfileName += "r_" + to_string(r_vector.size()) + "_" + to_string(rho) + "_" + to_string(theta);
		cout << "r_vector.size() = " << r_vector.size() << " ,r_vector.size()/2 = " << r_vector.size() / 2 << ", rho = " << rho << ",theta=" << theta << endl;
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 5000 * (-b));
		pt1.y = cvRound(y0 + 5000 * (a));
		pt2.x = cvRound(x0 - 5000 * (-b));
		pt2.y = cvRound(y0 - 5000 * (a));
		cout<<pt1.x<<","<<pt1.y<<","<<pt2.x<<","<<pt2.y<<endl;
		line(inputImage, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
		r_pt1 = pt1;
		r_pt2 = pt2;
		max_right_step = 0;
	} else {
		if (max_right_step < max_step)
			line(inputImage, r_pt1, r_pt2, Scalar(0, 255, 0), 3, CV_AA);
		max_right_step++;
	}
	/**
	 for (size_t i = 0; i < lines.size(); i++) {
	 float rho = lines[i][0], theta = lines[i][1];
	 std::cout << theta << std::endl;
	 //	if(theta>1.4&&theta<1.85){//internet
	 if (theta > 1.34 && theta < 1.75) {
	 continue;
	 }
	 laneDet.drawLane(inputImage, rho, theta);
	 }
	 */
//	imwrite("/home/kangyong/image/debug/" + debugfileName + ".jpg", inputImage);
	return 0;
}
void LaneDetection::drawLane(Mat& image, const double& rho, const double& theta) {
	Point pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a * rho, y0 = b * rho;
	pt1.x = cvRound(x0 + 5000 * (-b));
	pt1.y = cvRound(y0 + 5000 * (a));
	pt2.x = cvRound(x0 - 5000 * (-b));
	pt2.y = cvRound(y0 - 5000 * (a));
	line(image, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
}


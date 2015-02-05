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
const size_t queue_container_number = 10; //参考链长度
bool record = false;

//test mothed
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
	//根据预设的车道线宽度对图片做横向处理
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

/**
 *
 */
void drawLines(Mat& image, const Point& p1, const Point& p2, int height, int width) {
	cout << height << "," << width << endl;
	for (int i = 0; i < 30; i++) {
		if (p2.x - p1.x == 0) {
			continue;
		}
		double x = p2.x + (height - p2.y) * (p2.x - p1.x) / (p2.y - p1.y);
		Point pp1(x - width / 25, height);
		Point pp2(x + width / 25, height);
		line(image, pp1, pp2, Scalar(0, 255, 0), 3, CV_AA);
		height -= 10;
	}

//	line(image, p1, p2, Scalar(0, 255, 0), 3, CV_AA);
}
//std::queue<vector<Vec2f>> q_left;
list<double> list_left; //识别直线的半径集合
list<double> list_right;
//std::queue<int> q;
size_t cou = 0;
int LaneDetection::laneDetection(Mat& inputImage, Mat& src, ofstream& result_stream, list<Point>& points, string imageName) {
	if (!inputImage.data) {
		return -1;
	}

	Mat inputImage_gray;
//	cout<<inputImage.size()<<endl;
	//根据输入图片属性做灰度处理
	if (inputImage.channels() == 3)
		cvtColor(inputImage, inputImage_gray, CV_BGR2GRAY);
	else
		inputImage.copyTo(inputImage_gray);
	//	blur(src_gray, detected_edges, Size(1, 1));
	markLaneDetection(inputImage_gray, detected_edges, inputImage.cols / 50); //12
	int lowThreshold = 66;

//	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
	//canny边界识别
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * 3, kernel_size);
//	namedWindow("Canny",CV_WINDOW_AUTOSIZE);
	vector<Vec2f> lines;
	//houghlines识别直线
	HoughLines(detected_edges, lines, 1, CV_PI / 180, inputImage.cols / 12, 0, 0);

	//test
//	cout << "lines number: " << lines.size() << endl;
	vector<double> r_vector;	//识别的右侧直线集合（极坐标半径）
	vector<double> l_vector;	//识别的左侧直线集合（极坐标半径）
	map<double, double> my_map;	//极坐标下半径和弧度的对应
	cou++;
	string debugfileName = to_string(cou);
	string out_text;
	out_text = imageName;
	for (size_t i = 0; i < lines.size(); i++) {
		float rho = lines[i][0], theta = lines[i][1];
		//过滤直线
		if (theta > 1.3 && theta < 1.9) {		//1.34 1.799 filter lines
			continue;
		}
		if (rho > 0) {
			l_vector.push_back(theta);
//			drawLane(src, rho, theta);
		} else {
			r_vector.push_back(theta);
		}
		my_map[theta] = rho;
	}
	//排序，取中位数作为候选车道线
	sort(l_vector.begin(), l_vector.end());
	sort(r_vector.begin(), r_vector.end());

	/**左侧车道线处理*/
	if (l_vector.size() > 0) {
		double theta = l_vector[l_vector.size() / 2];
		double rho = my_map[l_vector[l_vector.size() / 2]];
		if (l_vector.size() > 5) {		//识别的直线较多的情况下填充
			if (list_left.size() < queue_container_number) {
				list_left.push_back(rho);
			}
		}
		debugfileName += "l_" + to_string(l_vector.size()) + "_" + to_string(rho) + "_" + to_string(theta);
		//和前queue_container_number的数值做比较
		if (l_vector.size() <= 3 && list_left.size() == queue_container_number && r_vector.size() != 0) {
			double sum = 0;
			for (list<double>::const_iterator ci = list_left.begin(); ci != list_left.end(); ++ci) {
				sum += *ci;
			}
			double mean = sum / queue_container_number;
			if (abs(mean - rho) > 70) {		//if difference is too big with last step, using last step point.
				drawLines(src, l_pt1, l_pt2, src.rows, src.cols);
//				line(src, l_pt1, l_pt2, Scalar(0, 255, 0), 3, CV_AA);
				points.push_back(l_pt1);
				points.push_back(l_pt2);
				out_text += " " + to_string(l_pt1.x * 2) + "," + to_string(l_pt1.y * 2) + "," + to_string(l_pt2.x * 2) + "," + to_string(l_pt2.y * 2);
				//compare test
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));
				if (record) {
					pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * src.rows;
					pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * src.rows;
				}
//				drawLines(src,pt1,pt2,src.rows,src.rows);
//				line(src, pt1, pt2, Scalar(0, 255, 255), 3, CV_AA);
				//end compare
			} else {
				Point pt1, pt2;

				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));
				if (record) {
					pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * src.rows;
					pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * src.rows;
				}
				drawLines(src, pt1, pt2, src.rows, src.rows);
//				line(src, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
				points.push_back(pt1);
				points.push_back(pt2);
				out_text += " " + to_string(pt1.x * 2) + "," + to_string(pt1.y * 2) + "," + to_string(pt2.x * 2) + "," + to_string(pt2.y * 2);
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
			if (record) {
				pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * src.rows;
				pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * src.rows;
			}
			drawLines(src, pt1, pt2, src.rows, src.rows);
//			line(src, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
			points.push_back(pt1);
			points.push_back(pt2);
			out_text += " " + to_string(pt1.x * 2) + "," + to_string(pt1.y * 2) + "," + to_string(pt2.x * 2) + "," + to_string(pt2.y * 2);
			l_pt1 = pt1;
			l_pt2 = pt2;
			max_left_step = 0;
			if (list_left.size() > 0)
				list_left.pop_front();
			list_left.push_back(rho);
		}
	} else {
		if (max_left_step < max_step) {		//小于阈值，继续沿用上一步的直线
//			line(src, l_pt1, l_pt2, Scalar(0, 255, 0), 3, CV_AA);
			drawLines(src, l_pt1, l_pt2, src.rows, src.cols);
			points.push_back(l_pt1);
			points.push_back(l_pt2);
			out_text += " " + to_string(l_pt1.x * 2) + "," + to_string(l_pt1.y * 2) + "," + to_string(l_pt2.x * 2) + "," + to_string(l_pt2.y * 2);
		}
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
		//和前queue_container_number的数值做比较
		if (r_vector.size() <= 3 && list_right.size() == queue_container_number && l_vector.size() != 0) {
			double sum = 0;
			for (list<double>::const_iterator ci = list_right.begin(); ci != list_right.end(); ++ci) {
				sum += *ci;
			}
			double mean = sum / queue_container_number;
			if (abs(mean - rho) > 70) {		//if difference is too big with last step, using last step point.
//				line(src, r_pt1, r_pt2, Scalar(0, 255, 0), 3, CV_AA);
				drawLines(src, l_pt1, l_pt2, src.rows, src.cols);
				points.push_back(r_pt1);
				points.push_back(r_pt2);
				out_text += " " + to_string(r_pt1.x * 2) + "," + to_string(r_pt1.y * 2) + "," + to_string(r_pt2.x * 2) + "," + to_string(r_pt2.y * 2);
				//compare test
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));
				if (record) {
					pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * src.rows;
					pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * src.rows;
				}
//				line(src, pt1, pt2, Scalar(0, 255, 255), 3, CV_AA);
//				drawLines(src,pt1,pt2,src.rows,src.rows);
				//end compare
			} else {
				Point pt1, pt2;

				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));
				if (record) {
					pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * src.rows;
					pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * src.rows;
				}
				drawLines(src, pt1, pt2, src.rows, src.rows);
//				line(src, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
				points.push_back(pt1);
				points.push_back(pt2);
				out_text += " " + to_string(pt1.x * 2) + "," + to_string(pt1.y * 2) + "," + to_string(pt2.x * 2) + "," + to_string(pt2.y * 2);
				l_pt1 = pt1;
				l_pt2 = pt2;
				max_right_step = 0;
				list_right.pop_front();
				list_right.push_back(rho);
			}
		} else {
//			cout << "r_vector.size() = " << r_vector.size() << " ,r_vector.size()/2 = " << r_vector.size() / 2 << ", rho = " << rho << ",theta=" << theta << endl;
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 5000 * (-b));
			pt1.y = cvRound(y0 + 5000 * (a));
			pt2.x = cvRound(x0 - 5000 * (-b));
			pt2.y = cvRound(y0 - 5000 * (a));
			if (record) {
				pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * src.rows;
				pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * src.rows;
			}
			drawLines(src, pt1, pt2, src.rows, src.rows);
//			line(src, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
			points.push_back(pt1);
			points.push_back(pt2);
			out_text += " " + to_string(pt1.x * 2) + "," + to_string(pt1.y * 2) + "," + to_string(pt2.x * 2) + "," + to_string(pt2.y * 2);
			r_pt1 = pt1;
			r_pt2 = pt2;
			max_right_step = 0;
		}
	} else {
		if (max_right_step < max_step) {
//			line(src, r_pt1, r_pt2, Scalar(0, 255, 0), 3, CV_AA);
			drawLines(src, r_pt1, r_pt2, src.rows, src.cols);
			points.push_back(r_pt1);
			points.push_back(r_pt2);
			out_text += " " + to_string(r_pt1.x * 2) + "," + to_string(r_pt1.y * 2) + "," + to_string(r_pt2.x * 2) + "," + to_string(r_pt2.y * 2);
		}
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
//	result_stream << out_text << endl;
	return 0;
}
int LaneDetection::laneDetection(Mat& inputImage, vector<Rect>& lineResult) {
	if (!inputImage.data) {
		return -1;
	}

	Mat inputImage_gray;
	//根据输入图片属性做灰度处理
	if (inputImage.channels() == 3)
		cvtColor(inputImage, inputImage_gray, CV_BGR2GRAY);
	else
		inputImage.copyTo(inputImage_gray);
	//	blur(src_gray, detected_edges, Size(1, 1));
	markLaneDetection(inputImage_gray, detected_edges, inputImage.cols / 50); //12
	int lowThreshold = 66;

//	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
	//canny边界识别
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * 3, kernel_size);
//	namedWindow("Canny",CV_WINDOW_AUTOSIZE);
	//houghlines识别直线
	vector<Vec2f> lines;
	HoughLines(detected_edges, lines, 1, CV_PI / 180, inputImage.cols / 12, 0, 0);

	//test
//	cout << "lines number: " << lines.size() << endl;
	vector<double> r_vector;	//识别的右侧直线集合（极坐标半径）
	vector<double> l_vector;	//识别的左侧直线集合（极坐标半径）
	map<double, double> my_map;	//极坐标下半径和弧度的对应
	for (size_t i = 0; i < lines.size(); i++) {
		float rho = lines[i][0], theta = lines[i][1];
		//过滤直线
		if (theta > 1.3 && theta < 1.9) {		//1.34 1.799 filter lines
			continue;
		}
		if (rho > 0) {
			l_vector.push_back(theta);
		} else {
			r_vector.push_back(theta);
		}
		my_map[theta] = rho;
	}
	//排序，取中位数作为候选车道线
	sort(l_vector.begin(), l_vector.end());
	sort(r_vector.begin(), r_vector.end());

	/**左侧车道线处理*/
	if (l_vector.size() > 0) {
		double theta = l_vector[l_vector.size() / 2];
		double rho = my_map[l_vector[l_vector.size() / 2]];
		if (l_vector.size() > 5) {		//识别的直线较多的情况下填充
			if (list_left.size() < queue_container_number) {
				list_left.push_back(rho);
			}
		}
		//和前queue_container_number的数值做比较
		if (l_vector.size() <= 3 && list_left.size() == queue_container_number && r_vector.size() != 0) {
			double sum = 0;
			for (list<double>::const_iterator ci = list_left.begin(); ci != list_left.end(); ++ci) {
				sum += *ci;
			}
			double mean = sum / queue_container_number;
			if (abs(mean - rho) > 70) {		//if difference is too big with last step, using last step point.
				Rect r;
				r.x = l_pt1.x;
				r.height = l_pt1.y;
				r.y = l_pt2.y;
				r.width = l_pt2.x;
				lineResult.push_back(r);
			} else {
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));

				pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * inputImage.rows / 0.4;
				pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * inputImage.rows / 0.4;

				Rect r;
				r.x = pt1.x;
				r.height = pt1.y;
				r.y = pt2.y;
				r.width = pt2.x;
				lineResult.push_back(r);
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
			pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * inputImage.rows / 0.4;
			pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * inputImage.rows / 0.4;

			Rect r;
			r.x = l_pt1.x;
			r.height = l_pt1.y;
			r.y = l_pt2.y;
			r.width = l_pt2.x;
			lineResult.push_back(r);
			l_pt1 = pt1;
			l_pt2 = pt2;
			max_left_step = 0;
			if (list_left.size() > 0)
				list_left.pop_front();
			list_left.push_back(rho);
		}
	} else {
		if (max_left_step < max_step) {		//小于阈值，继续沿用上一步的直线
//			line(src, l_pt1, l_pt2, Scalar(0, 255, 0), 3, CV_AA);
			Rect r;
			r.x = l_pt1.x;
			r.height = l_pt1.y;
			r.y = l_pt2.y;
			r.width = l_pt2.x;
			lineResult.push_back(r);
		}
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
		//和前queue_container_number的数值做比较
		if (r_vector.size() <= 3 && list_right.size() == queue_container_number && l_vector.size() != 0) {
			double sum = 0;
			for (list<double>::const_iterator ci = list_right.begin(); ci != list_right.end(); ++ci) {
				sum += *ci;
			}
			double mean = sum / queue_container_number;
			if (abs(mean - rho) > 70) {		//if difference is too big with last step, using last step point.
//				line(src, r_pt1, r_pt2, Scalar(0, 255, 0), 3, CV_AA);
				Rect r;
				r.x = r_pt1.x;
				r.height = r_pt1.y;
				r.y = r_pt2.y;
				r.width = r_pt2.x;
				lineResult.push_back(r);
			} else {
				Point pt1, pt2;

				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 5000 * (-b));
				pt1.y = cvRound(y0 + 5000 * (a));
				pt2.x = cvRound(x0 - 5000 * (-b));
				pt2.y = cvRound(y0 - 5000 * (a));
				pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * inputImage.rows / 0.4;
				pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * inputImage.rows / 0.4;
				Rect r;
				r.x = l_pt1.x;
				r.height = l_pt1.y;
				r.y = l_pt2.y;
				r.width = l_pt2.x;
				lineResult.push_back(r);
				l_pt1 = pt1;
				l_pt2 = pt2;
				max_right_step = 0;
				list_right.pop_front();
				list_right.push_back(rho);
			}
		} else {
//			cout << "r_vector.size() = " << r_vector.size() << " ,r_vector.size()/2 = " << r_vector.size() / 2 << ", rho = " << rho << ",theta=" << theta << endl;
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 5000 * (-b));
			pt1.y = cvRound(y0 + 5000 * (a));
			pt2.x = cvRound(x0 - 5000 * (-b));
			pt2.y = cvRound(y0 - 5000 * (a));
			pt1.y = cvRound(y0 + 5000 * (a)) + 0.6 * inputImage.rows / 0.4;
			pt2.y = cvRound(y0 - 5000 * (a)) + 0.6 * inputImage.rows / 0.4;
//			line(src, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
			Rect r;
			r.x = pt1.x;
			r.height = pt1.y;
			r.y = pt2.y;
			r.width = pt2.x;
			lineResult.push_back(r);
			r_pt1 = pt1;
			r_pt2 = pt2;
			max_right_step = 0;
		}
	} else {
		if (max_right_step < max_step) {
//			line(src, r_pt1, r_pt2, Scalar(0, 255, 0), 3, CV_AA);
			Rect r;
			r.x = r_pt1.x;
			r.height = r_pt1.y;
			r.y = r_pt2.y;
			r.width = r_pt2.x;
			lineResult.push_back(r);
		}
		max_right_step++;
	}
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


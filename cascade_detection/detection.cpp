/*
 * detection.cpp
 *
 *  Created on: 2014年11月5日
 *      Author: qin
 */
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** 函数声明 */
void detectAndDisplay(Mat frame);

/** 全局变量 */
string cascade_name = "/home/qin/cv_model/cascade.xml";
CascadeClassifier car_cascade;
string window_name = "Capture - Car detection";


int canny_thread = 30;
Mat CannyThreshold(Mat &gray){
	Mat detected_edges;
	/// 使用 3x3内核降噪
	blur(gray, detected_edges, Size(3,3));
	/// 运行Canny算子
	Canny(detected_edges, detected_edges, canny_thread, canny_thread*3, 3);
	return detected_edges;
}

/** @主函数 */
int main(int argc, const char** argv) {
        Mat frame, gray, edge;

        //-- 1. 加载级联分类器文件
        if (!car_cascade.load(cascade_name)) {
                printf("--(!)Error loading\n");
                return -1;
        }

        //-- 2. 载入图像
        string pic = argv[1];

        VideoCapture vc;
        vc.open(pic);

        //frame = cv::imread(pic);

        //cvtColor(frame, gray, CV_RGB2GRAY);
	//edge = CannyThreshold(gray);
	//cvtColor(edge, edge, CV_GRAY2BGR);
        //resize(edge, edge, Size(frame.cols/2, frame.rows/2));
        //detectAndDisplay(edge);
        //-- 3. 对当前帧使用分类器进行检测
        vc >> frame;
        while(!frame.empty()){
                resize(frame, frame, Size(frame.cols/3, frame.rows/3));
                detectAndDisplay(frame);
                vc >> frame;
                waitKey(1);
        }
        return 0;
}

/** @函数 detectAndDisplay */
void detectAndDisplay(Mat frame) {
        std::vector<Rect> cars;
        Mat frame_gray;

        cvtColor(frame, frame_gray, CV_BGR2GRAY);
        equalizeHist(frame_gray, frame_gray);

        //-- 多尺寸检测人脸
        car_cascade.detectMultiScale(
                        frame_gray,
                        cars,
                        1.2,
                        10,
                        0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING,
                        Size(1, 1),
                        Size(200, 200));
        //cout << cars.size()<< endl;
        for (size_t i = 0; i < cars.size(); i++) {
                rectangle(frame, cars[i], Scalar(0, 0, 255));
        }
        //-- 显示结果图像
        imshow(window_name, frame);
        //waitKey();
}


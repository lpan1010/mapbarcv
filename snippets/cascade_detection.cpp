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
void resize_rect(Mat& original_frame, Mat& resized_frame, Rect& rect);
void echo_rect(int _frame_num, vector<Rect> & cars);
/** 全局变量 */
string cascade_name = "/home/qin/cv_model/cascade.xml";
CascadeClassifier car_cascade;
string window_name = "Capture - Car detection";
Mat _frame;
int _frame_num;

/** @主函数 */
int main(int argc, const char** argv) {
        Mat gray, edge;
        assert(argc == 2);
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
        vc >> _frame;
        Mat small;
        _frame_num = 1;
        while(!_frame.empty()){
                resize(_frame, small, Size(_frame.cols/3, _frame.rows/3));
                detectAndDisplay(small);
                vc >> _frame;
                waitKey(1);
                _frame_num++;
        }
        return 0;
}

/** @函数 detectAndDisplay */
void detectAndDisplay(Mat frame) {
        std::vector<Rect> cars;
        Mat frame_gray;

        cvtColor(frame, frame_gray, CV_BGR2GRAY);
        equalizeHist(frame_gray, frame_gray);

        //-- 多尺寸检测
        car_cascade.detectMultiScale(
                        frame_gray,
                        cars,
                        1.2,
                        12,
                        0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING,
                        Size(1, 1),
                        Size(400, 400));

        for (size_t i = 0; i < cars.size(); i++) {
                rectangle(frame, cars[i], Scalar(0, 255, 0), 3);
                resize_rect(_frame, frame, cars[i]);
                rectangle(_frame, cars[i], Scalar(0, 255, 0), 3);
        }
        //echo_rect(_frame_num, cars);
        //-- 显示结果图像
        //resize(frame, frame, Size(frame.cols*2, frame.rows*2));
        imshow(window_name, _frame);
        imshow("Small", frame);
        //waitKey();
}

void echo_rect(int _frame_num, vector<Rect> & cars){
        cout << _frame_num << ".jpg";
        for(size_t i = 0; i < cars.size(); ++i){
                cout << ' ' << cars[i].x;
                cout << ',' << cars[i].y;
                cout << ',' << cars[i].width;
                cout << ',' << cars[i].height;
        }
        cout << endl;
}

void resize_rect(Mat& original_frame, Mat& resized_frame, Rect& rect){
        double height_resize_ratio = (double)original_frame.rows / resized_frame.rows;
        double width_resize_ratio = (double)original_frame.cols/ resized_frame.cols;
        double relative_pos_x = (double)rect.x / resized_frame.rows;
        double relative_pos_y = (double)rect.y / resized_frame.cols;

        rect.x = original_frame.rows * relative_pos_x;
        rect.y = original_frame.cols * relative_pos_y;
        rect.width = rect.width * width_resize_ratio;
        rect.height= rect.height * height_resize_ratio;
}


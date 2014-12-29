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
#include "main.h"

using namespace std;
using namespace cv;

/** 函数声明 */
void detectAndDisplay(Mat frame);
void detectAndDisplay_double(Mat frame);
void resize_rect(Mat& original_frame, Mat& resized_frame, Rect& rect);
void resize_rect_double(Rect& rect, Mat& resized_frame);
void echo_rect(int _frame_num, vector<Rect> & cars);
/** 全局变量 */
string cascade_name = "/home/qin/cv_model_2/cascade.xml";
CascadeClassifier car_cascade;
string window_name = "Capture - Car detection";
Mat _frame;
Mat _frame_double_size;

int _frame_num;

Net *n;

void load_network(const string& model_file){
        n  = load_model(model_file);
}

void mat2matrix(Mat& mat, Matrix2D& m){
        int size = mat.rows*mat.cols;
        m.resize(1, size);
        if (mat.isContinuous()) {
                for(int i = 0; i < size; ++i){
                        m.set(0, i, (int)mat.data[i]/ 255.0);
                        //cout << m.get(0, i) << endl;
                }
        }else{

        }
}

bool network_say_yes(Mat& small){
        //imshow("Network working on", small);
        Matrix2D in(1, 900), out(1, 1);
        mat2matrix(small, in);
        n->forward(in, out);
        //cout << out.get(0,0) << endl;
        //waitKey();
        return out.get(0,0) > 0;
}

/** @主函数 */
int main(int argc, const char** argv) {
        load_network("/home/qin/model.txt");

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

        waitKey();
        waitKey();
        //frame = cv::imread(pic);

        //cvtColor(frame, gray, CV_RGB2GRAY);
	//edge = CannyThreshold(gray);
	//cvtColor(edge, edge, CV_GRAY2BGR);
        //resize(edge, edge, Size(frame.cols/2, frame.rows/2));
        //detectAndDisplay(edge);
        //-- 3. 对当前帧使用分类器进行检测
        vc >> _frame;
        _frame_double_size = _frame.clone();
        Mat small;
        _frame_num = 1;
        while(!_frame.empty()){
                // Small
                resize(_frame, small, Size(_frame.cols/4, _frame.rows/4));
                detectAndDisplay(small);

                // 远距离范围
                Rect rect(0, _frame.rows/4, _frame.cols, _frame.rows/4);
                Mat double_size = Mat(_frame_double_size, rect);
                resize(double_size, double_size, Size(double_size.cols/2, double_size.rows/2));
                detectAndDisplay_double(double_size);

                imshow(window_name, _frame);
                imshow("Small", small);
                imshow("Double Size", double_size);

                vc >> _frame;
                _frame_double_size = _frame.clone();
                waitKey(1);
                _frame_num++;
        }
        return 0;
}


/** @函数 detectAndDisplay */
void detectAndDisplay_double(Mat frame) {
        std::vector<Rect> cars;
        Mat frame_gray;


        //resize(frame, frame, Size(_frame.cols, _frame.rows));

        //cvtColor(frame, frame_gray, CV_BGR2GRAY);

        //imshow("before quh", frame_gray);
        //equalizeHist(frame_gray, frame_gray);
        //imshow("after quh", frame_gray);

        //-- 多尺寸检测
        car_cascade.detectMultiScale(
                        frame,
                        cars,
                        1.3,
                        4,
                        0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING,
                        Size(29, 29),
                        Size(80, 80));

        for (size_t i = 0; i < cars.size(); i++) {
                //Mat small = Mat(frame_gray, cars[i]);
                //cv::resize(small, small, Size(30, 30));
                //if (network_say_yes(small)){
                        rectangle(frame, cars[i], Scalar(0, 255, 0), 3);
                        resize_rect_double(cars[i], frame);
                        rectangle(_frame, cars[i], Scalar(0, 255, 0), 3);
/*                }else{
                        rectangle(frame, cars[i], Scalar(0, 0, 255), 3);
                        resize_rect_double(cars[i], frame);
                        //rectangle(_frame, cars[i], Scalar(255, 255, 0), 3);
                }*/
        }
        echo_rect(_frame_num, cars);
        //-- 显示结果图像
        //resize(frame, frame, Size(frame.cols*2, frame.rows*2));

        //waitKey();
}
/** @函数 detectAndDisplay */
void detectAndDisplay(Mat frame) {
        std::vector<Rect> cars;
        Mat frame_gray;


        //resize(frame, frame, Size(_frame.cols, _frame.rows));
        //cvtColor(frame, frame_gray, CV_BGR2GRAY);
        ///home/qin/car_rear/video/1362909815.mov
        //equalizeHist(frame_gray, frame_gray);


        //-- 多尺寸检测
        car_cascade.detectMultiScale(
                        frame,
                        cars,
                        1.2,
                        3,
                        0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING,
                        Size(35, 35),
                        Size(400, 400));

        for (size_t i = 0; i < cars.size(); i++){
                cout << "f....." << endl;
                cout << cars[i].x << endl
                     << cars[i].y << endl
                     << cars[i].width << endl
                     << cars[i].height << endl;
                //Mat small = Mat(frame_gray, cars[i]);
                //resize(small, small, Size(30, 30));
                //if (network_say_yes(small)){
                        rectangle(frame, cars[i], Scalar(0, 255, 0), 3);
                        resize_rect(_frame, frame, cars[i]);
                        rectangle(_frame, cars[i], Scalar(0, 255, 0), 3);
        /*}else{
                        rectangle(frame, cars[i], Scalar(0, 0, 255), 3);
                        resize_rect(_frame, frame, cars[i]);
                }*/
        }
        echo_rect(_frame_num, cars);
        //-- 显示结果图像
        //resize(frame, frame, Size(frame.cols*2, frame.rows*2));
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


void resize_rect_double(Rect& rect, Mat& resized_frame){
        double relative_pos_x = (double)rect.x / resized_frame.cols;
        double relative_pos_y = (double)rect.y / resized_frame.rows;

        rect.x = resized_frame.cols*2*relative_pos_x;
        rect.y = resized_frame.rows*2*(1+relative_pos_y);
        //rect.x = resized_frame.rows*2*(relative_pos_x+1);
        //rect.y = resized_frame.cols*2*relative_pos_y;
        rect.width = rect.width*2;
        rect.height = rect.height*2;
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


/*
 * main.cpp
 *
 *  Created on: 2014年11月6日
 *      Author: qin
 */
#include <string>
#include <fstream>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
using namespace std;

double ratio = -1.0;

typedef std::string String;

void split(const std::string &s, char delim, vector<std::string> &elems) {
        stringstream ss(s);
        string item;
        while (std::getline(ss, item, delim)) {
                elems.push_back(item);
        }
}

int parse_roi(String roi_str, Rect & roi) {
        vector<String> pos;
        split(roi_str, ',', pos);
        vector<int> pos_int;
        for (size_t i = 0; i < pos.size(); ++i) {
                pos_int.push_back(atoi(pos[i].c_str()));
        }
        if (pos_int[2] < 0 || pos_int[3] < 0) {
                return -1;
        }

        //cout << pos_int[0] <<','<< pos_int[1]<<',' << pos_int[2]<<',' << pos_int[3]<<endl;
        roi.x = pos_int[0];
        roi.y = pos_int[1];
        roi.width = pos_int[2];
        roi.height = pos_int[3];
        return 1;
}

int cut_pic(Mat& img, String& roi_str, Rect& roi) {
        int r = parse_roi(roi_str, roi);
        return r;
}

inline Mat load_pic(const String& dir, const String & name) {
        cout << "load file:" << dir + name << endl;
        return imread(dir + '/' + name);
}

inline void write_pic(const Mat& img, const String& path) {
        imwrite(path, img);
}

int main(int argc, char **argv) {
        char number_buffer[33];

        String meta_file_path = "/home/qin/car_rear/close_test/label_for_nn.ini";
        String pics_dir = "/home/qin/car_rear/trainset_pic_11-3/extract/";
        String output_dir = "/home/qin/car_rear/close_test/label_2014_11_09_for_nn_filled";

        vector<String> file_names;
        ifstream meta_file(meta_file_path.c_str());
        String line;
        while (getline(meta_file, line)) {
                vector<string> tokens;
                split(line, ' ', tokens);
                String pic_name = tokens[0];
                cout << "File:" << pic_name << endl;
                Mat img = load_pic(pics_dir, pic_name);
                Mat empty = Mat(Size(img.cols,img.rows), CV_8UC1, Scalar(0));
                for (size_t i = 1; i < tokens.size(); ++i) {
                        sprintf(number_buffer, "%d", i);
                        Rect rect;
                        if (cut_pic(img, tokens[i], rect) < 0){
                                continue;
                        }
                        rectangle(empty, rect, Scalar(255), CV_FILLED);
                }
                //imshow("fff", empty);
                //waitKey();
                imwrite(output_dir + '/' + pic_name, empty);
        }
        cout << "Mission Completed" << endl;
}


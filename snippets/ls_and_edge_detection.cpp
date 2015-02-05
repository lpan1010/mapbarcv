/*
 * main.cpp
 *
 *  Created on: 2014年11月5日
 *      Author: qin
 */
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>

#include <dirent.h>
#include <errno.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
using namespace std;

const String path_separator = "/";
int ls(String dir, vector<String> &files) {
        DIR *dp;
        struct dirent *dirp;
        if ((dp = opendir(dir.c_str())) == NULL) {
                cout << "Error(" << errno << ") opening " << dir << endl;
                return errno;
        }

        while ((dirp = readdir(dp)) != NULL) {
                String name = String(dirp->d_name);
                if (name == "." || name == "..") {
                        continue;
                }
                files.push_back(name);
        }
        closedir(dp);
        return 0;
}

//int canny_thread = 30;
//Mat CannyThreshold(Mat &gray){
//	Mat detected_edges;
//	/// 使用 3x3内核降噪
//	blur(gray, detected_edges, Size(3,3));
//	/// 运行Canny算子
//	Canny(detected_edges, detected_edges, canny_thread, canny_thread*3, 3);
//	return detected_edges;
//}

int main(int argc, char **argv) {
        string input_dir = "/home/qin/car_rear/trainset_pic_11-3/extract/";
        string output_dir = "/home/qin/car_rear/gray/";
        vector<string> file_names;
        Mat img, gray, edge;
        ls(input_dir, file_names);
        for (int i = 0; i < file_names.size(); ++i){
                string input_file = input_dir+file_names[i];
                string output_file = output_dir+file_names[i];
                cout << input_file << " --> ";
                cout << output_file << endl;
                img = imread(input_file);
                cvtColor(img, gray, CV_BGR2GRAY);
                imwrite(output_file, gray);
        }
}





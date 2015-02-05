/*
 * main.cpp
 *
 *  Created on: 2014年11月4日
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


using namespace std;
using namespace cv;
typedef std::string String;

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

int main(int argc, char **argv) {
        String dir = "/home/qin/car_rear/training_set/";
        vector<String> file_names;
        ls(dir, file_names);
        String out_dir = "/tmp/new_label";
        for (int var = 0; var < file_names.size(); ++var) {
                Mat m = cv::imread(dir+path_separator+file_names[var]);
                Mat fav;
                //imshow("pic", m);
                //waitKey();
                cv::resize(m, fav, Size(125, 100));
                cv::imwrite(out_dir+path_separator+file_names[var], fav);
        }
}





//============================================================================
// Name        : TrainData4Car.cpp
// Author      : KangYong
// Version     :
// Copyright   : Your copyright notice
//============================================================================

#include <iostream>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <errno.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
using namespace cv;

/**
 * video保存截图
 */
void video2image(const string &inputfile, const string& outputFolder) {
	Mat src;
	VideoCapture video;
	if (!video.open(inputfile))
		return;
	int count = -1;
	int width = 0, height = 0, fps = 0, fourcc = 0, frameCount = 0;
	width = static_cast<int>(video.get(CV_CAP_PROP_FRAME_WIDTH));
	height = static_cast<int>(video.get(CV_CAP_PROP_FRAME_HEIGHT));
	cout << video.get(CV_CAP_PROP_FPS) << endl;
	fps = static_cast<int>(video.get(CV_CAP_PROP_FPS)); //幀率
	frameCount = static_cast<int>(video.get(CV_CAP_PROP_FRAME_COUNT)); //幀总数
	cout << "[" << width << "X" << height << "],fps=" << fps << ",frameCount="
			<< frameCount << endl;

	for (;;) {
		video >> src;
		if (src.empty())
			break;
		Mat resize_img(720, 1280, src.type());
		if (src.cols != 720 || src.rows != 1280) {
			resize(src, resize_img, resize_img.size(), 0, 0, INTER_LINEAR);
		}
		count++;
//		cout<<count % 20 <<endl;
		if (count % 20 != 0) {
			continue;
		}
		stringstream ss;
		ss << count;
		string name = ss.str();
		cout << outputFolder + name.c_str() + ".jpg" << endl;
		imwrite(outputFolder + name.c_str() + ".jpg", resize_img);
	}
}
int findExtFile(string dir, vector<string> &files, const string& ext) {
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		string name = string(dirp->d_name);
		if (name.rfind(ext) == -1) {
			continue;
		}
		files.push_back(name);
	}
	closedir(dp);
	return 0;
}
void videotrainData() {
	string root = "/home/share/待标注视频/";
	string token[7] =
			{ "new", "hk", "myh", "ssl", "xdy", "zhangxin", "zhaozz" };
	for (int i = 0; i < 1; i++) {
		//		cout<<root+token[i]+"/"<<endl;
		vector<string> file;
		string fileRoot = root + token[i] + "/";
		findExtFile(fileRoot, file, "mov");
		for (int j = 0; j < file.size(); j++) {
			string vodieFile = fileRoot + file[j];
			cout << fileRoot + file[j] << endl;
			string fileFolder = vodieFile.substr(0, vodieFile.rfind('.'));
			cout << fileFolder << endl;
			mkdir(fileFolder.c_str(), 0777);
			video2image(vodieFile, fileFolder + "/");
		}
	}
}
vector<string> split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
void testjust() {
	string videoFile = "/home/share/1362384151.mov";
	VideoCapture video;
	if (!video.open(videoFile))
		return;
	Mat src;

//	string imageFolder = "/home/kangyong/image/1362384151/";

	string inputFile = "/home/kangyong/image/1362384151/result.txt";
	ifstream infile(inputFile.c_str());
	string line;
	string key, value;
	while (getline(infile, line)) {
		cout << line << endl;
		int offset = line.find(" ");
		key = line.substr(0, offset);
		stringstream s_s(key);
		int frameNum;
		s_s >> frameNum;
		cout << sizeof(frameNum) << endl;
		video.set(CV_CAP_PROP_POS_FRAMES, frameNum);
		video >> src;
		if (src.empty())
			continue;
		istringstream iss(line.substr(offset + 1, line.length()));
		do {
			string slice;
			iss >> slice;
			if (slice.empty()) {
				continue;
			}
			vector<string> points;
			split(slice, ',', points);
			if (points.size() == 4) {
				cv::Point p1, p2;
				stringstream(points[0]) >> p1.x;
				stringstream(points[1]) >> p1.y;
				stringstream(points[2]) >> p2.x;
				stringstream(points[3]) >> p2.y;
				cv::Point p3;
				p3.x = p1.x + p2.x;
				p3.y = p1.y + p2.y;
				rectangle(src, p1, p3, Scalar(0, 255, 0), 2, 8, 0);
			}
		} while (iss);
		imshow("test", src);
		waitKey(0);

//		rectangle(img, Point)
	}

}

void checklabel() {
	string imageFolder = "/home/share/待标注视频/hk/1362909815_t/";
	string inputFile = "/home/share/lbs.txt";
	ifstream infile(inputFile.c_str());
	string line;
	string key, value;
	cv::Mat img;
	while (getline(infile, line)) {
		cout << line << endl;
		int offset = line.find(" ");
		key = line.substr(0, offset);
//		key += ".jpg";
		img = cv::imread(imageFolder + key.c_str());
		istringstream iss(line.substr(offset + 1, line.length()));
		do {
			string slice;
			iss >> slice;
			if (slice.empty()) {
				continue;
			}
			vector<string> points;
			split(slice, ',', points);
			if (points.size() == 4) {
				cv::Point p1, p2;
				stringstream(points[0]) >> p1.x;
				stringstream(points[1]) >> p1.y;
				stringstream(points[2]) >> p2.x;
				stringstream(points[3]) >> p2.y;
				cv::Point p3;
				p3.x = p1.x + p2.x;
				p3.y = p1.y + p2.y;
				rectangle(img, p1, p3, Scalar(0, 255, 0), 2, 8, 0);
			}
		} while (iss);
		imshow("test", img);
		waitKey(0);

//		rectangle(img, Point)
	}

}
int main() {
//	videotrainData();
	//	video2image("/home/share/待标注视频/dxz/1364352112.mov", "/home/share/待标注视频/dxz/1364352112/");
	checklabel();
	return 0;
}

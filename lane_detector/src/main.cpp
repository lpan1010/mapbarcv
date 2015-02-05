#include "laneDetection.h"
#include "logcplus/logCplus.h"
#include <iostream>

#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
LaneDetection laneDet;
ofstream result_stream;
//laneResult* laneResult::instance = 0;
int testVideo() {

	LogCPlus log;
	log.openLogFile("/home/kangyong/image/video.log");
	log.setLogEnable();
	Mat src, src_gray;
	/*
	 src = imread("/home/kangyong/image/srcgray.png");
	 testLaneDet(src, src_gray);
	 ********/
	string videoFile;
	videoFile = "/home/kangyong/image/test.mov";
//	videoFile = "/home/kangyong/image/IMG_0591.MOV";
//	videoFile = "/home/share/待标注视频/zhangxin/1364790372.mov";
	VideoCapture video;
	if (!video.open(videoFile))
		return 1;
	video >> src;
	cout << src.size() << endl;
	int width = 0, height = 0, fps = 0, fourcc = 0, frameCount = 0;
	width = static_cast<int>(video.get(CV_CAP_PROP_FRAME_WIDTH));
	height = static_cast<int>(video.get(CV_CAP_PROP_FRAME_HEIGHT));
	fps = static_cast<int>(video.get(CV_CAP_PROP_FPS)); //幀率
	frameCount = static_cast<int>(video.get(CV_CAP_PROP_FRAME_COUNT)); //幀总数
	cout << "[" << width << "X" << height << "],fps=" << fps << ",frameCount=" << frameCount << endl;
	int cc = 1;
	for (;;) {
		video >> src;
		if (src.empty())
			break;
		double rh = 0.6;
		if (src.cols > 800) {
			pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
			rh = 0.6;
		}
		log.writeLogLine(cc++);
		Mat dst1(src, Rect(0, rh * src.rows, src.cols, (1 - rh) * src.rows));
		/**
		 frameCout++;
		 name<<frameCout;
		 imwrite("/home/kangyong/image/realroadImage/"+name.str()+".jpg",src);
		 **/
		clock_t begin = clock();
		list<Point> points;
//		laneDet.laneDetection(dst1, dst1, result_stream, points);


		vector<Rect> r;
		laneDet.laneDetection(dst1, r);
		for (int i = 0; i < r.size(); i++) {
			Point pp1(r[i].x, r[i].height);
			Point pp2(r[i].width, r[i].y);
			line(src, pp1, pp2, Scalar(0, 255, 0), 3, CV_AA);
			cout<<r[i].x<<","<<r[i].height<<","<<r[i].width<<","<<r[i].y<<endl;
		}
		pair<vector<Point>,vector<Point>> p;


//		cout << 1000 * (double) (clock() - begin) / CLOCKS_PER_SEC << endl;
		imshow("output", src);
		waitKey(20);
	}
	log.closeLogFile();
	return 0;
}
template<typename T>
std::string to_string(T const & value) {
	stringstream sstr;
	sstr << value;
	return sstr.str();
}
string cascade_name = "/home/kangyong/image/car_data/model_HOG/cascade.xml";
CascadeClassifier car_cascade;
bool isBetweenLines(Rect& r, list<Point>& points) {
	cout << points.size() << endl;
	if (points.size() == 4) {
		Point p1, p2;
		for (list<Point>::iterator it = points.begin(); it != points.end(); ++it) {
			p1 = *it;
			++it;
			p2 = *it;
			//(y-y1)(x2-x1)=(x-x1)(y2-y1)
			//x=(y-y1)(x2-x1)/(y2-y1)+x1
			if (p2.y - p1.y == 0) {
				continue;
			}
			double x1 = (r.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
			++it;
			p1 = *it;
			++it;
			p2 = *it;
			if (p2.y - p1.y == 0) {
				continue;
			}
			double x2 = (r.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
			if ((r.x > x1 && r.x < x2) || (r.x > x2 && r.x < x1)) {
				return true;
			} else {
				return false;
			}
		}
	}
	return false;
}
void detectCar(Mat& frame, list<Point>& points) {
	std::vector<Rect> cars;
	car_cascade.detectMultiScale(frame, cars, 1.1, 15, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, Size(10, 10), Size(200, 200));
	Point textPos;
	string text;
	for (size_t i = 0; i < cars.size(); i++) {
//		if (isBetweenLines(cars[i], points)) {			//
//			rectangle(frame, cars[i], Scalar(255, 0, 0), 3);
//		} else {
//			rectangle(frame, cars[i], Scalar(0, 0, 255), 3);
//		}
		rectangle(frame, cars[i], Scalar(0, 0, 255), 3);
		text = to_string(cars[i].height) + "x" + to_string(cars[i].width);
		textPos.x = cars[i].x;
		textPos.y = cars[i].y;
		putText(frame, text, textPos, 2, 0.65, Scalar(0, 0, 255));
	}

}
int testVideoLaneAndCar() {
	Mat src, src_gray;
	string videoFile;
	videoFile = "/home/share/待标注视频/zhangxin/1364790372.mov";
	videoFile = "/home/kangyong/image/test.mov";
	VideoCapture video;
	if (!video.open(videoFile))
		return 1;
	if (!car_cascade.load(cascade_name)) {
		printf("--(!)Error loading\n");
		return -1;
	}
	video >> src;
	cout << src.size() << endl;
	int width = 0, height = 0, fps = 0, fourcc = 0, frameCount = 0;
	width = static_cast<int>(video.get(CV_CAP_PROP_FRAME_WIDTH));
	height = static_cast<int>(video.get(CV_CAP_PROP_FRAME_HEIGHT));
	fps = static_cast<int>(video.get(CV_CAP_PROP_FPS)); //幀率
	frameCount = static_cast<int>(video.get(CV_CAP_PROP_FRAME_COUNT)); //幀总数
	cout << "[" << width << "X" << height << "],fps=" << fps << ",frameCount=" << frameCount << endl;
	for (;;) {
		video >> src;
		if (src.empty())
			break;
		double rh = 0.6;
		if (src.cols > 800) {
			pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
			rh = 0.5;
		}
		Mat dst1(src, Rect(0, rh * src.rows, src.cols, (1 - rh) * src.rows));
		clock_t begin = clock();
		list<Point> points;
		laneDet.laneDetection(dst1, dst1, result_stream, points);
		resize(src, src, Size(src.cols / 2, src.rows / 2));
		detectCar(src, points);
//		cout << 1000 * (double) (clock() - begin) / CLOCKS_PER_SEC << endl;
		imshow("output", src);
		waitKey(1);
	}
	return 0;
}
int testPicture() {
	LogCPlus log;
	log.openLogFile("/home/kangyong/image/video.log");
	Mat src, src_gray;
	src = imread("/home/kangyong/image/realroadImage/1317.jpg");
	src = imread("/home/kangyong/image/realroadImage/2244.jpg");
//	src = imread("/home/kangyong/image/realroadImage/3033.jpg");
//	src = imread("/home/kangyong/image/realroadImage/7890.jpg");
//	src = imread("/home/kangyong/image/realroadImage/610.jpg");
//	src = imread("/home/kangyong/image/f00004.png");

	cout << "orignal size = " << src.size() << endl;
	cout << src.cols << endl;
//	Mat resize_img(360, 640, src.type());
	double rh = 0.6;
//	if (src.cols != 640 || src.rows != 360) {
//		resize(src, resize_img, resize_img.size(), 0, 0, INTER_LINEAR);
//		rh = 0.6;
//	}
	pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
	/**
	 if (src.cols > 800) {
	 pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
	 imwrite("/home/kangyong/image/1.jpg", src);
	 rh = 0.5;
	 }
	 cout << "after pyrdown  size = " << src.size() << endl;
	 Mat dst1(src, Rect(0, rh * src.rows, src.cols, (1 - rh) * src.rows));
	 */
	Mat dst1(src, Rect(0, rh * src.rows, src.cols, (1 - rh) * src.rows));
	list<Point> points;
	laneDet.laneDetection(dst1, dst1, result_stream, points);
	imshow("picOutput", src);
	waitKey(0);

	log.closeLogFile();
	return 0;
}
void testlog() {
	LogCPlus log;
	log.openLogFile("/home/kangyong/image/video.log");
	log.writeLog(23224);
	log.closeLogFile();
}
void test_hog() {
//	Mat img=imread("/home/kangyong/image/1113.jpg");
//	HOGDescriptor hog;
//	vector<float> d;
//	hog.compute(img,d);

}
//template <typename T>
//std::string to_string(T const & value){
//	stringstream sstr;
//	sstr<<value;
//	return sstr.str();
//}

void testPictures(const string& inputFolder, const string& txtName, const string& output) {
	ifstream inFile((inputFolder + txtName).c_str());
	string line, imageName;
	result_stream.open(output.c_str(), ios::out | ios::app);
	while (getline(inFile, line)) {
		imageName = line.substr(0, line.find(" "));

		cout << endl << "image name = " << imageName << endl;
		Mat src_gray, src;
		src = imread(inputFolder + imageName);
		double rh = 0.6;
		if (src.cols > 800) {
			pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
			rh = 0.6;
		}
		/**坐标系移动后
		 cvtColor(src, src_gray, CV_BGR2GRAY);
		 for (int j = 0; j < rh * src_gray.rows; j++) {
		 unsigned char *ptrRowSrc = src_gray.ptr<uchar>(j);
		 for (int i = 0; i < src.cols; ++i) {
		 ptrRowSrc[i] = (unsigned char) 255;
		 }
		 }
		 */

		Mat dst1(src, Rect(0, rh * src.rows, src.cols, (1 - rh) * src.rows));
		cout << src_gray.size() << endl;
		list<Point> points;
		laneDet.laneDetection(dst1, dst1, result_stream, points);

		imshow("picOutput", src);
//		imshow("picOutput1", src_gray);
		waitKey(1);
	}
	result_stream.close();
}
void detectBenchmark() {
	string folder = "/home/kangyong/image/realroadImage/";
	string txtlist = "imageTag.txt";
	string resultTxt = folder + txtlist + ".result";
	testPictures(folder, txtlist, resultTxt);
}
int main(int argc, char **argv) {
//	detectBenchmark();

//	testVideo();
	testVideoLaneAndCar();

	list<Point> points;
	Mat img11;
	img11 = imread("/home/share/realroadImage/6924.jpg");
	resize(img11, img11, Size(img11.cols / 4, img11.rows / 4));
	for (int i = 0; i < 1000; i++) {
		detectCar(img11, points);
		imshow("est", img11);
		waitKey(2);
	}
//	testPicture();
//	testlog();
//	test_tutorials();
//	test_orb();

	/*
	 double d = 32.4;
	 string name("hello");
	 //	name.append(sizeof(double),d);
	 string s =to_string(d);
	 cout<<s;
	 */
	return 0;
}


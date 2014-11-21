#include "laneDetection.h"
#include "logcplus/logCplus.h"
#include <iostream>
#include "benchmark.h"

#include "lanePoint.h"
using namespace std;
LaneDetection laneDet;
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
	videoFile = "/home/kangyong/image/road.avi";
	videoFile = "/home/kangyong/image/IMG_0593.MOV";
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
		//	imshow("ROI",dst1);
		/**
		 frameCout++;
		 name<<frameCout;
		 imwrite("/home/kangyong/image/realroadImage/"+name.str()+".jpg",src);
		 **/
		clock_t begin = clock();
		laneDet.laneDetection(dst1, src_gray);
		cout << 1000 * (double) (clock() - begin) / CLOCKS_PER_SEC << endl;
		imshow("output", src);
		waitKey(2);
	}
	log.closeLogFile();
	return 0;
}
int testPicture() {
	LogCPlus log;
	log.openLogFile("/home/kangyong/image/video.log");
	Mat src, src_gray;
	src = imread("/home/kangyong/image/realroadImage/1317.jpg");
	src = imread("/home/kangyong/image/realroadImage/8984.jpg");
//	src = imread("/home/kangyong/image/realroadImage/3033.jpg");
//	src = imread("/home/kangyong/image/realroadImage/7890.jpg");
//	src = imread("/home/kangyong/image/realroadImage/610.jpg");
//	src = imread("/home/kangyong/image/f00004.png");

	cout << "orignal size = " << src.size() << endl;
	cout << src.cols << endl;
	Mat resize_img(360, 640, src.type());
	double rh = 0.6;
	if (src.cols != 640 || src.rows != 360) {
		resize(src, resize_img, resize_img.size(), 0, 0, INTER_LINEAR);
		rh = 0.5;
	}
	cout << "resize   size = " << resize_img.size() << endl;
	/**
	 if (src.cols > 800) {
	 pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
	 imwrite("/home/kangyong/image/1.jpg", src);
	 rh = 0.5;
	 }
	 cout << "after pyrdown  size = " << src.size() << endl;
	 Mat dst1(src, Rect(0, rh * src.rows, src.cols, (1 - rh) * src.rows));

	 */
	Mat dst1(resize_img, Rect(0, rh * resize_img.rows, resize_img.cols, (1 - rh) * resize_img.rows));
	laneDet.laneDetection(dst1, src_gray);
	imshow("picOutput", resize_img);
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

void testBenchmark() {
	Mat inputImage = imread("/home/kangyong/image/realroadImage/8559.jpg");
	benchmark::getInstance().loadBenchmark("/home/kangyong/image/lane_labeler/imageTag.txt");
	Point p1;
	p1.x = 950;
	p1.y = 715;
	Point p2;
	p2.x = 627;
	p2.y = 337;
//	benchmark::createInstance().evaluate("8559.jpg", p1, p2);

//	line(inputImage, p1, p2, Scalar(0, 255, 0), 8, CV_AA);
//	imshow("outputImage", inputImage);
//	waitKey(0);
}
void testPictures(const string inputFolder, const string txtName) {
	ifstream inFile((inputFolder + txtName).c_str());
	string line, imageName;
	while (getline(inFile, line)) {
		imageName = line.substr(0, line.find(" "));

		cout << endl << "image name = " << imageName << endl;
		Mat dst1, src_gray;
		dst1 = imread(inputFolder + imageName);
		laneDet.laneDetection(dst1, src_gray, imageName);

		imshow("picOutput", dst1);
		waitKey(0);
	}
}
int main(int argc, char **argv) {
//	testBenchmark();
	string folder = "/home/kangyong/image/realroadImage/";
	string txt = "imageTag.txt";
	benchmark::getInstance().loadBenchmark("/home/kangyong/image/lane_labeler/imageTag.txt");
	testPictures(folder, txt);

//	testVideo();

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


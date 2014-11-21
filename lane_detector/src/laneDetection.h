#ifndef  __LANE_DETECTION__
#define __LANE_DETECTION__

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include "logcplus/logCplus.h"

using namespace cv;
#define PI 3.1415926

class LaneDetection {
private:
	Mat dst, detected_edges;
	int ratio = 3;
	int kernel_size = 3;
//	double PI = 3.1415926;
	double theta_min = PI * 45.0 / 180.0;
	double theta_max = PI * 135.0 / 180;

	Point l_pt1, l_pt2;// last point
	Point r_pt1, r_pt2;
	int max_step = 10;
	int max_left_step = 0;
	int max_right_step = 0;
public:
	void markLaneDetection(Mat& inputImage, Mat& outputImage, int laneLen);
//	int laneDetection(Mat& inputImage, Mat& inputImage_gray,LogCPlus& log);
	int laneDetection(Mat& inputImage, Mat& inputImage_gray,std::string imageName="");
	void drawLane(Mat& image, const double& rho, const double& theta);

};

#endif

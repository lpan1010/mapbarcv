#ifndef  __LANE_DETECTION__
#define __LANE_DETECTION__

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include <list>

#include "logcplus/logCplus.h"

using namespace cv;
using namespace std;
#define PI 3.1415926

class LaneDetection {
private:
	Mat dst, detected_edges;
	int ratio = 3;
	int kernel_size = 3;
//	double PI = 3.1415926;
	double theta_min = PI * 45.0 / 180.0;
	double theta_max = PI * 135.0 / 180;

	Point l_pt1, l_pt2; // last point
	Point r_pt1, r_pt2;
	int max_step = 20;
	int max_left_step = 0;
	int max_right_step = 0;
public:
	/**
	 * 根据车道的宽度对输入图片做预处理
	 */
	void markLaneDetection(Mat& inputImage, Mat& outputImage, int laneLen);
//	int laneDetection(Mat& inputImage, Mat& inputImage_gray,LogCPlus& log);
	/**
	 *车道识别
	 */
	int laneDetection(Mat& inputImage, Mat& inputImage_gray, std::ofstream& result_stream, list<Point>& points, std::string imageName = "");
	/**
	 * 车道识别接口
	 * inputImage 输入的待识别图片
	 * lines 识别的车道线。取巧方法，用Rect中（x,height)(width,y)表示车道线的两个点P1(x1,y1)P2(x2,y2)
	 */
	int laneDetection(Mat& inputImage, vector<Rect>& lines) ;

	/**
	* @param inputImage 输入图片
	* @param lines 识别返回的车道线。pair里每个vector代表一条车道线上的点的集合。
	*/
	int laneDetection(Mat& inputImage, pair<vector<Point>,vector<Point>>& lines) ;

	/**
	 * 图片上标注车道。依据极坐标下的两个参数
	 */
	void drawLane(Mat& image, const double& rho, const double& theta);

};

#endif

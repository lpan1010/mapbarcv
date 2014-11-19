#ifndef CAR_DETECTOR_RULE_TL_DETECTOR_HPP_
#define CAR_DETECTOR_RULE_TL_DETECTOR_HPP_

#include <vector>
#include <list>

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>


using namespace cv;
using namespace std;

class TailLightDetector {
public:
    TailLightDetector();
    void detector(
                    Mat& img,
                    Mat& blured,
                    Mat& gray,
                    Mat& hsv,
                    Mat& canny,
                    list< pair<KeyPoint, KeyPoint> > &ret);

protected:
    void find_pairs(list< pair<KeyPoint, KeyPoint> > & pairs, vector<KeyPoint> & keypoints);
    void filter_out_pairs(list< pair<KeyPoint, KeyPoint> > &pairs,
                    Mat& canny,
                    list< pair<KeyPoint, KeyPoint> > &ret);

    Ptr<FeatureDetector> blob_detector;
    SimpleBlobDetector::Params params;
    int pair_thread;
};

#endif

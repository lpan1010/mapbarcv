#include "tail_light_detector.hpp"

TailLightDetector::TailLightDetector(){
        //
	params.minDistBetweenBlobs = 1.0f;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = true;
	params.filterByCircularity = false;
	params.filterByArea = true;
	params.blobColor = 255;
	// TODO configs
	params.minArea = 2.0f;
	params.maxArea = 100.0f;

	blob_detector= new SimpleBlobDetector(params);
	blob_detector->create("SimpleBlob");
	pair_thread = 10;
}

void TailLightDetector::detector(Mat& img, Mat& blured, Mat& gray, Mat& hsv, Mat& canny, list < pair<KeyPoint, KeyPoint> > & ret){
        Mat mask1, mask0, mask;
        // 获得过滤红色的mask
	inRange(hsv, Scalar(165, 40, 0),Scalar(180, 255, 255), mask1);
	inRange(hsv, Scalar(0, 40, 0),Scalar(5, 255, 255), mask0);
	cv::bitwise_or(mask0, mask1, mask);

	imshow("Mask", mask);
	// 探测红点
	vector<KeyPoint> red_points;
	blob_detector->detect(mask, red_points);

	// 寻找符合条件的红点对
	list< pair<KeyPoint, KeyPoint> > pairs;
	find_pairs(pairs, red_points);
	filter_out_pairs(pairs, canny, ret);
}

void TailLightDetector::filter_out_pairs(list< pair<KeyPoint, KeyPoint> > &pairs, Mat& canny, list< pair<KeyPoint, KeyPoint> > &ret){
        for (std::list<pair<KeyPoint, KeyPoint> >::iterator it=pairs.begin(); it != pairs.end(); ++it){
		KeyPoint& first = (*it).first;
		KeyPoint& second = (*it).second;

		float rect_height = abs(first.pt.x - second.pt.x)/2;
		float rect_min_x = min(first.pt.x, second.pt.x);
		float rect_min_y = min(first.pt.y, second.pt.y) - 10;

		// 上三分之一直接跳过
		if (rect_min_y < canny.rows*0.33){
		 	continue;
		}

		float distance = abs(first.pt.x-second.pt.x);

		// TODO 距离处理需要进一步确定
		if (distance > 200 || distance < 20){
			continue;
		}

		Rect ROI(max(0.0f, rect_min_x - 10),
			 rect_min_y,
			 min(abs(first.pt.x-second.pt.x)+20, canny.cols - rect_min_x),
			 min(rect_height, canny.rows - rect_min_y));
		Mat smallImage = Mat(canny, ROI);
		vector<Vec2f> lines;
		HoughLines(smallImage, lines, 1, CV_PI/180, smallImage.cols/2, 0, 0 );
		bool isRear = false;
		for(size_t i = 0; i < lines.size(); i++ ){
			float theta = lines[i][1];
			if (theta > 3.14/2-0.01 && theta < 3.14/2 + 0.01){
				isRear = true;
				break;
			}else if (theta > 0-0.01 && theta < 0 + 0.01){
				isRear = true;
				break;
			}
		}
		if (isRear){
		        ret.push_back((*it));
		}
	}
}

void TailLightDetector::find_pairs(list< pair<KeyPoint, KeyPoint> > & pairs, vector<KeyPoint> & keypoints){
	for (size_t i = 0; i < keypoints.size(); i++) {
		for (size_t j = i; j < keypoints.size(); j++) {
			if (i == j){
				continue;
			}
			float y0 = keypoints[i].pt.y;
			float y1 = keypoints[j].pt.y;
			if (abs(y0-y1)< pair_thread){
				pairs.push_back(make_pair(keypoints[i],keypoints[j]));
			}
		}
	}
}



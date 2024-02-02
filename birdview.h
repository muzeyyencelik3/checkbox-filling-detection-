#ifndef BIRDVIEW_H
#define BIRDVIEW_H

#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/features2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;


class birdview {
public:
    void alignImages(cv::Mat& im1, cv::Mat& im2, cv::Mat& im1Reg, cv::Mat& h);
 
private:
    const int MAX_FEATURES = 500;
    const float GOOD_MATCH_PERCENT = 0.15f;
    const float DISTANCE_THRESHOLD = 0.3f; // Eþik deðeri
};

#endif // BIRDVIEW_H
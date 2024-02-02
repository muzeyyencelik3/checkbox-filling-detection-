// birdview.cpp
#include "birdview.h"

BirdView::BirdView() {
    // Add any necessary initialization for birdview transformation
    // For now, using a simple perspective transformation matrix
    cv::Point2f srcPoints[4] = {{0, 0}, {640, 0}, {640, 480}, {0, 480}};
    cv::Point2f dstPoints[4] = {{100, 100}, {540, 100}, {540, 380}, {100, 380}};
    perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);
}

BirdView::~BirdView() {
    // Add any necessary cleanup for birdview transformation
}

cv::Mat BirdView::transform(const cv::Mat& input) {
    // Implement birdview transformation logic here
    cv::Mat result;
    cv::warpPerspective(input, result, perspectiveMatrix, cv::Size(640, 480));

    return result;
}

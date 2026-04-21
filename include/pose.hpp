#pragma once
#include <opencv2/opencv.hpp>

namespace slam {

class PoseEstimator {
public:
    PoseEstimator(double focal = 718.8560, cv::Point2d pp = cv::Point2d(607.1928, 185.2157));

    bool estimate(const std::vector<cv::KeyPoint>& kp1, 
                  const std::vector<cv::KeyPoint>& kp2,
                  const std::vector<cv::DMatch>& matches,
                  cv::Mat& R, cv::Mat& t,
                  std::vector<cv::DMatch>& inliers);

private:
    double focal_;
    cv::Point2d pp_;
};

} // namespace slam

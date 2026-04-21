#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

namespace slam {

class FeatureModule {
public:
    FeatureModule(int num_features = 300);

    void detectCPU(const cv::Mat& frame,
                   std::vector<cv::KeyPoint>& keypoints,
                   cv::Mat& descriptors);

    void detectGPU(const cv::Mat& frame,
                   std::vector<cv::KeyPoint>& keypoints,
                   cv::Mat& descriptors);

private:
    cv::Ptr<cv::ORB> orb_cpu_;
    cv::Ptr<cv::ORB> orb_gpu_;
};

} // namespace slam
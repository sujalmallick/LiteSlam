#include "feature.hpp"

namespace slam {

FeatureModule::FeatureModule(int num_features) {
    orb_cpu_ = cv::ORB::create(num_features);
    orb_gpu_ = cv::ORB::create(num_features);
}

void FeatureModule::detectCPU(const cv::Mat& frame,
                              std::vector<cv::KeyPoint>& keypoints,
                              cv::Mat& descriptors) {

    cv::Mat gray;
    if (frame.channels() == 3)
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    else
        gray = frame;

    orb_cpu_->detectAndCompute(gray, cv::noArray(), keypoints, descriptors);
}

void FeatureModule::detectGPU(const cv::Mat& frame,
                              std::vector<cv::KeyPoint>& keypoints,
                              cv::Mat& descriptors) {
    // Fallback to CPU (no CUDA OpenCV available)
    detectCPU(frame, keypoints, descriptors);
}

} // namespace slam
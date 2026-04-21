#include "feature.hpp"

namespace slam {

FeatureModule::FeatureModule(int num_features) {
    // 🔥 Controlled feature count (fast + stable)
    orb_cpu_ = cv::ORB::create(300);
    orb_gpu_ = cv::ORB::create(300);
}

void FeatureModule::detectCPU(const cv::Mat& frame,
                              std::vector<cv::KeyPoint>& keypoints,
                              cv::Mat& descriptors) {

    cv::Mat gray;

    if (frame.channels() == 3)
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    else
        gray = frame;

    // 🔥 ORB detection
    orb_cpu_->detectAndCompute(gray, cv::noArray(), keypoints, descriptors);

    // 🔥 HARD LIMIT keypoints (CRITICAL for FPS)
    if (!descriptors.empty() && keypoints.size() > 200) {
        keypoints.resize(200);
        descriptors = descriptors.rowRange(0, 200).clone();
    }
}

void FeatureModule::detectGPU(const cv::Mat& frame,
                              std::vector<cv::KeyPoint>& keypoints,
                              cv::Mat& descriptors) {
    // ⚠️ No CUDA build → fallback
    detectCPU(frame, keypoints, descriptors);
}

} // namespace slam
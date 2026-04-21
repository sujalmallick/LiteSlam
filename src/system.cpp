#include "system.hpp"
#include <filesystem>

namespace slam {

SLAMSystem::SLAMSystem() {}

SLAMSystem::~SLAMSystem() {
    std::cout << "Finalizing SLAM system..." << std::endl;
    std::filesystem::create_directories("data");
    metrics_.saveToCSV("data/metrics.csv");
    tracker_.saveTUM("data/trajectory.txt");
}

void SLAMSystem::processFrame(cv::Mat& frame) {
    static int frame_num = 0;
    frame_num++;

    metrics_.updateFrame();

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    std::vector<cv::KeyPoint> keypoints;
    std::vector<cv::DMatch> matches;
    std::vector<cv::DMatch> inliers;

    cv::Mat descriptors;

    if (use_gpu_) {
        metrics_.startTimer("Feature Detection (GPU)");
        feature_engine_.detectGPU(gray, keypoints, descriptors);
        metrics_.stopTimer("Feature Detection (GPU)");

        if (!prev_descriptors_gpu_.empty()) {
            metrics_.startTimer("Feature Matching (GPU)");
            matcher_engine_.matchGPU(prev_descriptors_gpu_, descriptors, matches);
            metrics_.stopTimer("Feature Matching (GPU)");
        }
        prev_descriptors_gpu_ = descriptors;
    } else {
        metrics_.startTimer("Feature Detection (CPU)");
        feature_engine_.detectCPU(gray, keypoints, descriptors);
        metrics_.stopTimer("Feature Detection (CPU)");

        if (!prev_descriptors_cpu_.empty()) {
            metrics_.startTimer("Feature Matching (CPU)");
            matcher_engine_.matchCPU(prev_descriptors_cpu_, descriptors, matches);
            metrics_.stopTimer("Feature Matching (CPU)");
        }
        prev_descriptors_cpu_ = descriptors;
    }

    // Debug print
    if (frame_num % 30 == 0) {
        std::cout << "[Frame " << frame_num
                  << "] KP: " << keypoints.size()
                  << " | Matches: " << matches.size();
    }

    // Pose estimation
    if (!prev_keypoints_.empty() && matches.size() > 15) {
        cv::Mat R, t;
        if (pose_engine_.estimate(prev_keypoints_, keypoints, matches, R, t, inliers)) {
            tracker_.update(R, t);

            if (frame_num % 30 == 0) {
                std::cout << " | ✓ Pose OK | Path=" << tracker_.getPath().size();
            }
        } else {
            if (frame_num % 30 == 0) {
                std::cout << " | ✗ Pose FAILED";
            }
        }
    }

    if (frame_num % 30 == 0) std::cout << std::endl;

    // Visualization (correct signature)
    visualizer_.update(frame, keypoints, inliers, tracker_.getPath(), metrics_, use_gpu_);
    visualizer_.show();

    prev_keypoints_ = keypoints;
}

} // namespace slam
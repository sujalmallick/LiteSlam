#pragma once
#include <opencv2/opencv.hpp>

#include "feature.hpp"
#include "matcher.hpp"
#include "pose.hpp"
#include "trajectory.hpp"
#include "visualization.hpp"
#include "metrics.hpp"

namespace slam {

class SLAMSystem {
public:
    SLAMSystem();
    ~SLAMSystem();

    void processFrame(cv::Mat& frame);

    void toggleMode() { use_gpu_ = !use_gpu_; }
    bool isGPUMode() const { return use_gpu_; }

private:
    FeatureModule feature_engine_;
    MatcherModule matcher_engine_;
    PoseEstimator pose_engine_;
    TrajectoryTracker tracker_;
    Visualizer visualizer_;
    Metrics metrics_;

    bool use_gpu_ = false;

    std::vector<cv::KeyPoint> prev_keypoints_;
    cv::Mat prev_descriptors_cpu_;
    cv::Mat prev_descriptors_gpu_;
};

}
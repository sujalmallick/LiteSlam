#include "system.hpp"
#include <filesystem>
#include <chrono>
#include "cuda_kernels.hpp"

namespace slam {

SLAMSystem::SLAMSystem() {}

SLAMSystem::~SLAMSystem() {
    std::cout << "Finalizing SLAM system..." << std::endl;
    std::filesystem::create_directories("data");
    metrics_.saveToCSV("data/metrics.csv");
    tracker_.saveTUM("data/trajectory.txt");
}

// 🔥 NEW FUNCTION
void SLAMSystem::enableCudaPreprocessing(bool enable) {
    use_cuda_preprocessing_ = enable;
}
void SLAMSystem::toggleDisplayMode() {
    show_color_ = !show_color_;
}
void SLAMSystem::processFrame(cv::Mat& frame) {
    static int frame_num = 0;
    frame_num++;

    metrics_.updateFrame();

    // ================================
    // CPU vs CUDA PREPROCESSING
    // ================================

    cv::Mat gray_cpu, gray_cuda;

    // CPU timing
    auto cpu_start = std::chrono::high_resolution_clock::now();
    cv::cvtColor(frame, gray_cpu, cv::COLOR_BGR2GRAY);
    auto cpu_end = std::chrono::high_resolution_clock::now();

    double cpu_time =
        std::chrono::duration<double, std::milli>(cpu_end - cpu_start).count();

    // CUDA timing
    auto gpu_start = std::chrono::high_resolution_clock::now();
    runCudaGrayscale(frame, gray_cuda);
    auto gpu_end = std::chrono::high_resolution_clock::now();

    double gpu_time =
        std::chrono::duration<double, std::milli>(gpu_end - gpu_start).count();

    if (frame_num % 30 == 0) {
        std::cout << "[Timing] CPU: " << cpu_time
                  << " ms | CUDA: " << gpu_time << " ms\n";
    }

    // 🔥 SELECT PIPELINE INPUT
    cv::Mat gray;
   
if (use_cuda_preprocessing_)
    gray = gray_cuda;
else
    gray = gray_cpu;

// 🔥 Choose display frame
cv::Mat display_frame;
if (show_color_)
    display_frame = frame;
else
    display_frame = gray;

    // ================================
    // FEATURE DETECTION + MATCHING
    // ================================

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

    // ================================
    // DEBUG
    // ================================

    if (frame_num % 30 == 0) {
        std::cout << "[Frame " << frame_num
                  << "] KP: " << keypoints.size()
                  << " | Matches: " << matches.size();
    }

    // ================================
    // POSE ESTIMATION
    // ================================

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

    // ================================
    // VISUALIZATION
    // ================================
visualizer_.update(display_frame, keypoints, inliers,
                   tracker_.getPath(), metrics_, use_gpu_);
    visualizer_.show();

    prev_keypoints_ = keypoints;
}

}
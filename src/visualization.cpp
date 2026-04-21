#include "../include/visualization.hpp"

namespace slam {

Visualizer::Visualizer(std::string window_name) : window_name_(window_name) {
    canvas_traj_ = cv::Mat::zeros(600, 600, CV_8UC3);
}

void Visualizer::update(const cv::Mat& frame,
                        const std::vector<cv::KeyPoint>& kps,
                        const std::vector<cv::DMatch>& matches,
                        const std::vector<cv::Point3f>& trajectory,
                        const Metrics& metrics,
                        bool gpu_mode) {

    // 🔥 ALWAYS clone frame (important)
    canvas_frame_ = frame.clone();

    // Draw keypoints
    for (const auto& kp : kps) {
        cv::circle(canvas_frame_, kp.pt, 2, cv::Scalar(0,255,0), -1);
    }

    // Simple HUD
    cv::putText(canvas_frame_, "FPS: " + std::to_string((int)metrics.getFPS()),
                cv::Point(10,30),
                cv::FONT_HERSHEY_SIMPLEX,
                0.7,
                cv::Scalar(0,255,0),
                2);

    // Trajectory
    canvas_traj_ = cv::Mat::zeros(600, 600, CV_8UC3);

    int cx = 300, cy = 300;

    for (size_t i = 1; i < trajectory.size(); i++) {
        cv::Point p1(cx + trajectory[i-1].x*50, cy + trajectory[i-1].z*50);
        cv::Point p2(cx + trajectory[i].x*50, cy + trajectory[i].z*50);

        cv::line(canvas_traj_, p1, p2, cv::Scalar(0,255,0), 2);
    }
}

void Visualizer::show() {
    // 🔥 ALWAYS show (no condition blocking)
    cv::imshow("Frame Output", canvas_frame_);
    cv::imshow("Trajectory Path", canvas_traj_);
}

}
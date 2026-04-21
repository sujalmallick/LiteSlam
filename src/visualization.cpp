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

    // Clone frame
    canvas_frame_ = frame.clone();

    // Draw keypoints
    for (const auto& kp : kps) {
        cv::circle(canvas_frame_, kp.pt, 2, cv::Scalar(0,255,0), -1);
    }

    // =======================
    // 🔥 HUD (TOP LEFT)
    // =======================

    // FPS
    cv::putText(canvas_frame_,
                "FPS: " + std::to_string((int)metrics.getFPS()),
                cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX,
                0.7,
                cv::Scalar(0,255,0),
                2);

    // Mode (CPU / GPU)
    std::string mode = gpu_mode ? "GPU" : "CPU";
    cv::Scalar mode_color = gpu_mode ? cv::Scalar(0,255,255) : cv::Scalar(255,200,0);

    cv::putText(canvas_frame_,
                "Mode: " + mode,
                cv::Point(10, 60),
                cv::FONT_HERSHEY_SIMPLEX,
                0.6,
                mode_color,
                2);

    // =======================
    // 🔥 CONTROLS (BOTTOM LEFT)
    // =======================

    cv::Mat overlay = canvas_frame_.clone();

    cv::rectangle(overlay,
                  cv::Point(10, canvas_frame_.rows - 70),
                  cv::Point(320, canvas_frame_.rows - 30),
                  cv::Scalar(30, 30, 30),
                  -1);

    cv::addWeighted(overlay, 0.7, canvas_frame_, 0.3, 0, canvas_frame_);

    cv::putText(canvas_frame_,
                "[C] CPU   [G] GPU   [Q] Quit",
                cv::Point(20, canvas_frame_.rows - 45),
                cv::FONT_HERSHEY_SIMPLEX,
                0.5,
                cv::Scalar(220, 220, 220),
                1);

    // =======================
    // 🔥 TRAJECTORY
    // =======================

    canvas_traj_ = cv::Mat::zeros(600, 600, CV_8UC3);

    int cx = 300, cy = 300;

    for (size_t i = 1; i < trajectory.size(); i++) {
        cv::Point p1(cx + trajectory[i-1].x * 50,
                     cy + trajectory[i-1].z * 50);

        cv::Point p2(cx + trajectory[i].x * 50,
                     cy + trajectory[i].z * 50);

        cv::line(canvas_traj_, p1, p2, cv::Scalar(0,255,0), 2);
    }
}

void Visualizer::show() {
    cv::imshow("Frame Output", canvas_frame_);
    cv::imshow("Trajectory Path", canvas_traj_);
}

} // namespace slam
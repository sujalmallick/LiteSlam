#pragma once
#include <opencv2/opencv.hpp>
#include "metrics.hpp"

namespace slam {

class Visualizer {
public:
    Visualizer(std::string window_name = "SLAM GPU Project");

    void update(const cv::Mat& frame,
                const std::vector<cv::KeyPoint>& kps,
                const std::vector<cv::DMatch>& matches,
                const std::vector<cv::Point3f>& trajectory,
                const Metrics& metrics,
                bool gpu_mode);

    void show();

private:
    std::string window_name_;
    cv::Mat canvas_frame_;
    cv::Mat canvas_traj_;

    void drawHUD(cv::Mat& img,
                 const Metrics& metrics,
                 bool gpu_mode,
                 int num_kps,
                 int num_matches);

    void drawTrajectory(cv::Mat& img,
                        const std::vector<cv::Point3f>& path);
};

}
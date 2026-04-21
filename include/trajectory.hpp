#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

namespace slam {

class TrajectoryTracker {
public:
 TrajectoryTracker();
    void update(const cv::Mat& R, const cv::Mat& t);

    const std::vector<cv::Point3f>& getPath() const;

    void saveTUM(const std::string& path);

private:
    std::vector<cv::Point3f> path_;
    cv::Mat pose_ = cv::Mat::eye(4,4,CV_64F);
};

}
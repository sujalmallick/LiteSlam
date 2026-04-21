#include "trajectory.hpp"

namespace slam {

TrajectoryTracker::TrajectoryTracker() {
    path_.push_back(cv::Point3f(0,0,0));
}

void TrajectoryTracker::update(const cv::Mat& R, const cv::Mat& t) {
    cv::Point3f last = path_.back();

    cv::Point3f next(
        last.x + t.at<double>(0),
        last.y + t.at<double>(1),
        last.z + t.at<double>(2)
    );

    path_.push_back(next);
}

const std::vector<cv::Point3f>& TrajectoryTracker::getPath() const {
    return path_;
}

void TrajectoryTracker::saveTUM(const std::string&) {}

}
#include "pose.hpp"

namespace slam {

PoseEstimator::PoseEstimator(double focal, cv::Point2d pp)
    : focal_(focal), pp_(pp) {}

bool PoseEstimator::estimate(
    const std::vector<cv::KeyPoint>& kp1,
    const std::vector<cv::KeyPoint>& kp2,
    const std::vector<cv::DMatch>& matches,
    cv::Mat& R, cv::Mat& t,
    std::vector<cv::DMatch>& inliers) {

    if (matches.size() < 8) return false;

    std::vector<cv::Point2f> pts1, pts2;

    for (auto& m : matches) {
        pts1.push_back(kp1[m.queryIdx].pt);
        pts2.push_back(kp2[m.trainIdx].pt);
    }

    cv::Mat K = (cv::Mat_<double>(3, 3) <<
        focal_, 0, pp_.x,
        0, focal_, pp_.y,
        0, 0, 1);

    cv::Mat E = cv::findEssentialMat(pts1, pts2, K);

    if (E.empty()) return false;

    cv::recoverPose(E, pts1, pts2, K, R, t);

    inliers = matches;
    return true;
}

}
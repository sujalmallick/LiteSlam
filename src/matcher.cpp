#include "matcher.hpp"

namespace slam {

MatcherModule::MatcherModule() {
    matcher_cpu_ = cv::BFMatcher::create(cv::NORM_HAMMING);
}

void MatcherModule::matchCPU(const cv::Mat& desc1,
                             const cv::Mat& desc2,
                             std::vector<cv::DMatch>& good_matches) {

    std::vector<std::vector<cv::DMatch>> knn;
    matcher_cpu_->knnMatch(desc1, desc2, knn, 2);

    for (auto& m : knn) {
        if (m.size() < 2) continue;
        if (m[0].distance < ratio_threshold_ * m[1].distance) {
            good_matches.push_back(m[0]);
        }
    }
}

void MatcherModule::matchGPU(const cv::Mat& d1,
                             const cv::Mat& d2,
                             std::vector<cv::DMatch>& matches) {
    matchCPU(d1, d2, matches); // fallback
}

}
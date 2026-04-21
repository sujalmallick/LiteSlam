#pragma once
#include <opencv2/opencv.hpp>

namespace slam {

class MatcherModule {
public:
    MatcherModule();

    void matchCPU(const cv::Mat& desc1,
                  const cv::Mat& desc2,
                  std::vector<cv::DMatch>& matches);

    void matchGPU(const cv::Mat& desc1,
                  const cv::Mat& desc2,
                  std::vector<cv::DMatch>& matches);

private:
    cv::Ptr<cv::DescriptorMatcher> matcher_cpu_;
    cv::Ptr<cv::DescriptorMatcher> matcher_gpu_;
    float ratio_threshold_ = 0.75f;
};

}
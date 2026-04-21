#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <map>

namespace slam {

struct MetricLog {
    std::string name;
    double duration_ms;
};

class Metrics {
public:
    void updateFrame();

    void startTimer(const std::string& name);
    void stopTimer(const std::string& name);

    double getFPS() const;

    std::vector<MetricLog> getLogs() const;

    void saveToCSV(const std::string& path);

private:
    int frame_count_ = 0;
    double last_time_ = cv::getTickCount();
    double fps_ = 0;

    std::vector<MetricLog> logs_;
    std::map<std::string, double> timer_start_;
};

}
#include "metrics.hpp"
#include <fstream>

namespace slam {

void Metrics::updateFrame() {
    // 🔥 initialize once
    if (last_time_ == 0) {
        last_time_ = cv::getTickCount();
    }

    frame_count_++;

    double current_time = cv::getTickCount();
    double elapsed = (current_time - last_time_) / cv::getTickFrequency();

    if (elapsed >= 1.0) {
        fps_ = frame_count_ / elapsed;
        frame_count_ = 0;
        last_time_ = current_time;
    }
}

double Metrics::getFPS() const {
    return fps_;
}

void Metrics::startTimer(const std::string& name) {
    timer_start_[name] = cv::getTickCount();
}

void Metrics::stopTimer(const std::string& name) {
    double end = cv::getTickCount();
    double duration = (end - timer_start_[name]) / cv::getTickFrequency() * 1000.0;

    logs_.push_back({name, duration});
}

std::vector<MetricLog> Metrics::getLogs() const {
    return logs_;
}

void Metrics::saveToCSV(const std::string& path) {
    std::ofstream file(path);
    for (const auto& log : logs_) {
        file << log.name << "," << log.duration_ms << "\n";
    }
}

} // namespace slam
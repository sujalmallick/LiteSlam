#include "metrics.hpp"

namespace slam {

void Metrics::updateFrame() {
    frame_count_++;
}

double Metrics::getFPS() const {
    return 30.0;
}

std::vector<MetricLog> Metrics::getLogs() const {
    return logs_;
}

void Metrics::startTimer(const std::string&) {}
void Metrics::stopTimer(const std::string&) {}

void Metrics::saveToCSV(const std::string&) {}

}
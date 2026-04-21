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

    // ── Always ensure canvas_frame_ is BGR (3-channel) ──
    // This is the fix: no matter if frame is gray or color,
    // we always work on a 3-channel mat so HUD colors stay correct.
    if (frame.channels() == 1) {
        cv::cvtColor(frame, canvas_frame_, cv::COLOR_GRAY2BGR);
    } else {
        canvas_frame_ = frame.clone();
    }

    // Draw keypoints (always green, always visible)
    for (const auto& kp : kps) {
        cv::circle(canvas_frame_, kp.pt, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
    }

    // ─────────────────────────────────────────────
    //  HUD PANEL
    // ─────────────────────────────────────────────

    const int PX   = 14;
    const int PY   = 14;
    const int PW   = 230;
    const int LINE = 26;

    std::string modeStr = gpu_mode ? "GPU" : "CPU";
    int         fps     = static_cast<int>(metrics.getFPS());
    std::string featStr = gpu_mode ? "GPU"  : "CPU";
    std::string prepStr = gpu_mode ? "CUDA" : "CPU";

    // View string — read from original frame channels, not canvas
    std::string viewStr = (frame.channels() == 1) ? "Gray" : "Color";

    const int ROWS = 5;
    const int PH   = 20 + ROWS * LINE + 10;

    // Solid dark background
    cv::rectangle(canvas_frame_,
                  cv::Point(PX, PY),
                  cv::Point(PX + PW, PY + PH),
                  cv::Scalar(18, 18, 18), -1);

    // Outer border
    cv::rectangle(canvas_frame_,
                  cv::Point(PX, PY),
                  cv::Point(PX + PW, PY + PH),
                  cv::Scalar(70, 70, 70), 1);

    // Left cyan accent bar
    cv::rectangle(canvas_frame_,
                  cv::Point(PX, PY),
                  cv::Point(PX + 3, PY + PH),
                  cv::Scalar(0, 200, 255), -1);

    // Divider under title
    const int DIV_Y = PY + 32;
    cv::line(canvas_frame_,
             cv::Point(PX + 4, DIV_Y),
             cv::Point(PX + PW, DIV_Y),
             cv::Scalar(55, 55, 55), 1);

    const int TX = PX + 14;
    const int VX = PX + PW - 70;
    int       TY = PY + 24;

    // Title
    cv::putText(canvas_frame_, "SLAM",
                cv::Point(TX, TY),
                cv::FONT_HERSHEY_DUPLEX, 0.70,
                cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

    // Mode badge
    cv::Scalar modeColor = gpu_mode ? cv::Scalar(0, 200, 255)
                                    : cv::Scalar(0, 160, 255);
    cv::putText(canvas_frame_, modeStr,
                cv::Point(PX + PW - 52, TY),
                cv::FONT_HERSHEY_DUPLEX, 0.60,
                modeColor, 1, cv::LINE_AA);

    TY = DIV_Y + LINE - 2;

    auto row = [&](const std::string& label,
                   const std::string& value,
                   cv::Scalar valCol = cv::Scalar(230, 230, 230)) {
        cv::putText(canvas_frame_, label,
                    cv::Point(TX, TY),
                    cv::FONT_HERSHEY_DUPLEX, 0.46,
                    cv::Scalar(130, 130, 130), 1, cv::LINE_AA);
        cv::putText(canvas_frame_, value,
                    cv::Point(VX, TY),
                    cv::FONT_HERSHEY_DUPLEX, 0.46,
                    valCol, 1, cv::LINE_AA);
        TY += LINE;
    };

    cv::Scalar fpsCol = (fps >= 30) ? cv::Scalar(80,  255,  80)
                      : (fps >= 15) ? cv::Scalar(0,   200, 255)
                                    : cv::Scalar(60,   60, 255);
    row("FPS",     std::to_string(fps), fpsCol);
    row("Feature", featStr);
    row("Preproc", prepStr);
    row("View",    viewStr);

    // ─────────────────────────────────────────────
    //  CONTROLS BAR
    // ─────────────────────────────────────────────

    const int ROWS_H = canvas_frame_.rows;
    const int COLS_W = canvas_frame_.cols;
    const int BAR_H  = 36;

    cv::rectangle(canvas_frame_,
                  cv::Point(0, ROWS_H - BAR_H),
                  cv::Point(COLS_W, ROWS_H),
                  cv::Scalar(18, 18, 18), -1);

    cv::line(canvas_frame_,
             cv::Point(0,      ROWS_H - BAR_H),
             cv::Point(COLS_W, ROWS_H - BAR_H),
             cv::Scalar(65, 65, 65), 1);

    struct Key { std::string key; std::string desc; };
    const std::vector<Key> keys = {
        {"C","CPU"}, {"G","GPU"}, {"M","View"}, {"Q","Quit"}
    };

    int kx = 18;
    const int KY = ROWS_H - 11;

    for (const auto& k : keys) {
        cv::putText(canvas_frame_, k.key + ":",
                    cv::Point(kx, KY),
                    cv::FONT_HERSHEY_DUPLEX, 0.46,
                    cv::Scalar(0, 200, 255), 1, cv::LINE_AA);
        kx += 22;
        cv::putText(canvas_frame_, k.desc,
                    cv::Point(kx, KY),
                    cv::FONT_HERSHEY_DUPLEX, 0.46,
                    cv::Scalar(180, 180, 180), 1, cv::LINE_AA);
        kx += 68;

        if (k.key != "Q") {
            cv::line(canvas_frame_,
                     cv::Point(kx - 4, ROWS_H - BAR_H + 8),
                     cv::Point(kx - 4, ROWS_H - 8),
                     cv::Scalar(55, 55, 55), 1);
        }
    }

    // ─────────────────────────────────────────────
    //  TRAJECTORY
    // ─────────────────────────────────────────────

    canvas_traj_ = cv::Mat(600, 600, CV_8UC3, cv::Scalar(12, 12, 12));

    for (int g = 0; g <= 600; g += 50) {
        cv::line(canvas_traj_, cv::Point(g, 0), cv::Point(g, 600),
                 cv::Scalar(28, 28, 28), 1);
        cv::line(canvas_traj_, cv::Point(0, g), cv::Point(600, g),
                 cv::Scalar(28, 28, 28), 1);
    }

    cv::line(canvas_traj_, cv::Point(300, 0),   cv::Point(300, 600),
             cv::Scalar(48, 48, 48), 1);
    cv::line(canvas_traj_, cv::Point(0,   300), cv::Point(600, 300),
             cv::Scalar(48, 48, 48), 1);

    const int cx = 300, cy = 300;
    for (size_t i = 1; i < trajectory.size(); i++) {
        float t  = static_cast<float>(i) / trajectory.size();
        uchar gv = static_cast<uchar>(100 + 155 * t);
        cv::Point p1(cx + trajectory[i-1].x * 50, cy + trajectory[i-1].z * 50);
        cv::Point p2(cx + trajectory[i].x   * 50, cy + trajectory[i].z   * 50);
        cv::line(canvas_traj_, p1, p2, cv::Scalar(0, gv, 0), 2, cv::LINE_AA);
    }

    if (!trajectory.empty()) {
        cv::Point cur(cx + trajectory.back().x * 50,
                      cy + trajectory.back().z * 50);
        cv::circle(canvas_traj_, cur, 5,  cv::Scalar(0, 255, 120), -1, cv::LINE_AA);
        cv::circle(canvas_traj_, cur, 10, cv::Scalar(0, 160,  80),  1, cv::LINE_AA);
    }

    cv::putText(canvas_traj_, "Trajectory - XZ Plane",
                cv::Point(12, 22),
                cv::FONT_HERSHEY_DUPLEX, 0.46,
                cv::Scalar(90, 90, 90), 1, cv::LINE_AA);
}

void Visualizer::show() {
    cv::imshow("Frame Output",    canvas_frame_);
    cv::imshow("Trajectory Path", canvas_traj_);
}

} // namespace slam
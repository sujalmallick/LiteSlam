#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "system.hpp"

int main() {
    try {
        std::cout << std::unitbuf;
        std::cout << "=== Starting SLAM GPU Project ===" << std::endl;

        // 🔥 FIX: correct path (important)
        cv::VideoCapture cap("../data/sample_video.mp4");

        if (!cap.isOpened()) {
            std::cout << "Video not found, trying webcam..." << std::endl;
            cap.open(0);
            if (!cap.isOpened()) {
                std::cerr << "ERROR: Cannot open video or webcam!" << std::endl;
                return -1;
            }
        } else {
            std::cout << "✓ Video loaded successfully." << std::endl;
        }

        // Windows
        cv::namedWindow("Frame Output", cv::WINDOW_NORMAL);
        cv::namedWindow("Trajectory Path", cv::WINDOW_NORMAL);

        cv::resizeWindow("Frame Output", 800, 640);
        cv::resizeWindow("Trajectory Path", 700, 700);

        cv::moveWindow("Frame Output", 50, 50);
        cv::moveWindow("Trajectory Path", 900, 50);

        slam::SLAMSystem slam;

        cv::Mat frame;
        int frame_count = 0;

        while (true) {
            cap >> frame;

            if (frame.empty()) {
                std::cout << "✓ End of video reached." << std::endl;
                break;
            }

            cv::resize(frame, frame, cv::Size(640, 480));

            frame_count++;
            std::cout << "Frame OK\n";
            slam.processFrame(frame);

            if (frame_count % 30 == 0) {
                std::cout << "Processed " << frame_count << " frames..." << std::endl;
            }

            int key = cv::waitKey(30);

            if (key == 'q' || key == 27) {
                std::cout << "✓ Quit requested." << std::endl;
                break;
            }
            if (key == 'g' || key == 'G') {
                slam.toggleMode();
                std::cout << "✓ Mode toggled." << std::endl;
            }
        }

        std::cout << "\n=== Done ===\n";
        cv::destroyAllWindows();
        std::cin.get();
        return 0;
    }
    catch (...) {
        std::cerr << "Crash happened\n";
        std::cin.get();
        return -1;
    }
}
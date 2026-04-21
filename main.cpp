#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "system.hpp"

int main() {
    std::cout << cv::cuda::getCudaEnabledDeviceCount() << std::endl;
    try {
        std::cout << std::unitbuf;
        std::cout << "=== Starting SLAM GPU Project ===" << std::endl;

        // Load video
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

        cv::resizeWindow("Frame Output", 800, 600);
        cv::resizeWindow("Trajectory Path", 600, 600);

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

            // 🔥 Lower resolution (huge FPS boost)
            cv::resize(frame, frame, cv::Size(640, 480));

            frame_count++;

            // 🔥 Skip alternate frames (2x speed boost)
            if (frame_count % 2 != 0) continue;

            // SLAM processing
            slam.processFrame(frame);

            // 🔥 Reduce logging (avoid slowdown)
            if (frame_count % 100 == 0) {
                std::cout << "Processed " << frame_count << " frames..." << std::endl;
            }

            // Keyboard
            int key = cv::waitKey(1);

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
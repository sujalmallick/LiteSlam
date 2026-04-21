cmake_minimum_required(VERSION 3.10)
project(SLAM_Project)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    include
)

add_executable(SLAM_GPU_Project
    main.cpp
    src/system.cpp
    src/feature.cpp
    src/matcher.cpp
    src/pose.cpp
    src/trajectory.cpp
    src/metrics.cpp
    src/visualization.cpp
)

target_link_libraries(SLAM_GPU_Project
    ${OpenCV_LIBS}
)
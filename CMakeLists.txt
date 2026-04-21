cmake_minimum_required(VERSION 3.10)
project(SLAM_GPU_Project)

# 🔹 C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 🔥 Enable CUDA
enable_language(CUDA)
# CUDA include path (FIX)
include_directories("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v13.2/include")

# CUDA runtime library
link_directories("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v13.2/lib/x64")
# 🔹 OpenCV path (keep yours)
set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)

# 🔹 Include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    include
)

# 🔹 Source files (🔥 added CUDA files)
set(SOURCES
    main.cpp
    src/system.cpp
    src/feature.cpp
    src/matcher.cpp
    src/pose.cpp
    src/trajectory.cpp
    src/metrics.cpp
    src/visualization.cpp
    src/cuda_kernels.cpp
    src/cuda_kernels.cu
)

# 🔹 Create executable
add_executable(SLAM_GPU_Project ${SOURCES})

# 🔹 Link libraries
target_link_libraries(SLAM_GPU_Project
    ${OpenCV_LIBS}
    cudart
)

# 🔥CUDA architecture for RTX 3050
set_target_properties(SLAM_GPU_Project PROPERTIES
    CUDA_ARCHITECTURES 86
)
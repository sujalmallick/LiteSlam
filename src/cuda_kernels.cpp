#include "cuda_kernels.hpp"
#include <cuda_runtime.h>
#include <chrono>
// Declare a NORMAL wrapper function (not CUDA kernel)
extern "C" void launch_rgb2gray(unsigned char*, unsigned char*, int, int);
void runCudaGrayscale(const cv::Mat& input, cv::Mat& output) {
    auto start = std::chrono::high_resolution_clock::now();

    int width = input.cols;
    int height = input.rows;

    size_t inSize = width * height * 3;
    size_t outSize = width * height;

    unsigned char *d_in, *d_out;

    cudaMalloc(&d_in, inSize);
    cudaMalloc(&d_out, outSize);

    cudaMemcpy(d_in, input.data, inSize, cudaMemcpyHostToDevice);

    launch_rgb2gray(d_in, d_out, width, height);

    output.create(height, width, CV_8UC1);
    cudaMemcpy(output.data, d_out, outSize, cudaMemcpyDeviceToHost);

    cudaFree(d_in);
    cudaFree(d_out);

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "[CUDA] Grayscale Time: " << time_ms << " ms\n";
}
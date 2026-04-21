#include <cuda_runtime.h>

__global__ void rgb2gray(unsigned char* input, unsigned char* output, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    int idx = (y * width + x) * 3;

    unsigned char r = input[idx];
    unsigned char g = input[idx + 1];
    unsigned char b = input[idx + 2];

    output[y * width + x] = (r + g + b) / 3;
}

// 🔥 Wrapper function callable from C++
extern "C" void launch_rgb2gray(unsigned char* d_in,
                                 unsigned char* d_out,
                                 int width,
                                 int height) {

    dim3 block(16, 16);
    dim3 grid((width + 15) / 16, (height + 15) / 16);

    rgb2gray<<<grid, block>>>(d_in, d_out, width, height);
}
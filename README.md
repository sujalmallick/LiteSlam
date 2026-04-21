# Visual SLAM System

A modular Visual Simultaneous Localization and Mapping (SLAM) system built with C++ and OpenCV, supporting both CPU and GPU (CUDA) processing modes.

---

## What It Does

- Captures video from a hallway/indoor environment
- Detects and tracks visual features across frames
- Estimates camera pose (position + orientation)
- Records and visualizes the camera trajectory
- Benchmarks CPU vs GPU performance

---

## Project Structure

```
visual-slam/
├── src/
│   ├── main.cpp              # Entry point, video loop, keyboard input
│   ├── system.hpp/cpp        # Main orchestrator (SLAM pipeline)
│   ├── feature.hpp/cpp       # ORB feature detection
│   ├── matcher.hpp/cpp       # Feature matching (Lowe's Ratio Test)
│   ├── pose.hpp/cpp          # Pose estimation (Essential Matrix + RANSAC)
│   ├── trajectory.hpp/cpp    # Trajectory tracking (TUM format export)
│   ├── visualization.hpp/cpp # Live display windows + HUD
│   └── metrics.hpp/cpp       # FPS tracking, timing, CSV export
├── data/
│   ├── sample_video.mp4      # Input hallway video
│   ├── metrics.csv           # Performance benchmarking data
│   ├── trajectory.txt        # Camera path in TUM format
│   └── plot_performance.py   # Python script to visualize results
└── CMakeLists.txt            # Build configuration
```

---

## Dependencies

- C++17 or higher
- OpenCV 4.x
- CUDA (optional, for GPU mode)
- CMake 3.15+
- Python 3.x + matplotlib (for plotting)

---

##  Build Instructions

```bash
mkdir build && cd build
cmake ..
make -j4
```

> For CUDA support, ensure CUDA toolkit is installed and detected by CMake.

---

## ▶Usage

```bash
./visual_slam
```

### Keyboard Controls

| Key | Action |
|-----|--------|
| `C` | Switch to CPU mode |
| `G` | Switch to GPU mode |
| `Q` | Quit and save results |

---

## Pipeline Overview

```
Video Input (sample_video.mp4)
        ↓
    main.cpp  ←─── keyboard input
        ↓
   system.cpp  (orchestrates)
    ├── feature.cpp   → detect ORB keypoints
    ├── matcher.cpp   → match features between frames
    ├── pose.cpp      → estimate R & T (rotation + translation)
    ├── trajectory.cpp→ accumulate camera path
    └── visualization.cpp → render windows
        ↓
   metrics.cpp  → log FPS + timing
        ↓
Output: metrics.csv | trajectory.txt
```

---

## Output Files

| File | Description |
|------|-------------|
| `data/metrics.csv` | Per-frame FPS and processing times |
| `data/trajectory.txt` | Camera path in TUM format |

### Visualize Performance

```bash
cd data
python plot_performance.py
```

---

## Algorithms Used

| Module | Algorithm |
|--------|-----------|
| Feature Detection | ORB (Oriented FAST + Rotated BRIEF) |
| Feature Matching | Brute-Force + Lowe's Ratio Test |
| Pose Estimation | Essential Matrix + RANSAC |
| Trajectory Export | TUM RGB-D format |

---

## CPU vs GPU Comparison

- Switch modes live using `C` / `G` keys
- Performance data auto-saved to `metrics.csv`
- Use `plot_performance.py` to generate comparison graphs
- Tested on **NVIDIA RTX 3060**

---

## Results Display

Two live windows are shown during execution:
1. **Frame Output** — video feed with detected keypoints (green dots)
2. **Trajectory Path** — top-down bird's-eye view of camera movement

HUD displays: FPS, current mode (CPU/GPU), keypoint count

---

## Academic Note

- Trajectory output follows **TUM RGB-D benchmark** format
- Suitable for evaluation with standard SLAM benchmarking tools
- Modular architecture allows easy swapping of individual components


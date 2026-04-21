@echo off
REM Setup MSVC environment
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=x64

REM Navigate to build directory
cd /d "D:\workspace\Cloud Project\SLAM-GPU-PROJECT\build"

REM Clean previous build
echo Cleaning previous build...
del /Q CMakeCache.txt Makefile build.ninja 2>nul
rmdir /S /Q CMakeFiles 2>nul

REM Configure with Ninja
echo Configuring CMake...
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

REM Build
echo Building...
cmake --build .

echo.
echo Build complete! Running executable...
pause
.\SLAM_GPU_Project.exe
pause

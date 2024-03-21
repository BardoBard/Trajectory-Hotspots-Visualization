# Minimum CMake version
cmake_minimum_required(VERSION 3.1...3.23)

# c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Project name
project(Visualization)

set(PACKAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/packages)
set(TRAJECTORY_HOTSPOTS_SOLUTION_DIR ${PACKAGE_DIR}/Trajectory_Hotspots/Trajectory_Hotspots)
set(TRAJECTORY_HOTSPOTS_DIR ${PACKAGE_DIR}/Trajectory_Hotspots/Trajectory_Hotspots/Trajectory_Hotspots)

include_directories(
        ${PACKAGE_DIR}/packages/*/
)
# Add subdirectories
add_subdirectory(Trajectory-Hotspots-Parser)
add_subdirectory(Trajectory-Hotspots-Visualization)
add_subdirectory(Trajectory-Hotspots-Benchmark)
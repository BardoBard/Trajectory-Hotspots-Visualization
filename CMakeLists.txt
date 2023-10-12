# Minimum CMake version
cmake_minimum_required(VERSION 3.1...3.23)

# c++ 20
set(CMAKE_CXX_STANDARD 20)

# Project name
project(Visualization)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/packages/.?*/
${CMAKE_CURRENT_SOURCE_DIR}/packages/Trajectory_Hotspots/Trajectory_Hotspots/)

# Add subdirectories
add_subdirectory(Trajectory-Hotspots-Visualization)
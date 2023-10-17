# Minimum CMake version
cmake_minimum_required(VERSION 3.1...3.23)

# c++ 20
set(CMAKE_CXX_STANDARD 20)

# Project name
project(Visualization)

set(PACKAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/packages)

include_directories(
	${PACKAGE_DIR}/packages/.?*/
)

# Add subdirectories
add_subdirectory(Trajectory-Hotspots-Visualization)
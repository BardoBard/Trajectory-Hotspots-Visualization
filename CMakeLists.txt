# Minimum CMake version
cmake_minimum_required(VERSION 3.1...3.23)

# Project name
project(Visualization)

# Find CGAL and optional Qt5 component
find_package(CGAL REQUIRED Qt5)
find_package(OpenGL REQUIRED)

# Create a list of source files in the "Trajectory-Hotspots" directory
file(
  GLOB_RECURSE cppfiles
  ${CMAKE_CURRENT_SOURCE_DIR}/Trajectory-Hotspots/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Trajectory-Hotspots/*.h
)

# Include lib
include_directories(
${CGAL_INCLUDE_DIRS}
${QT5_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}/packages/
${CMAKE_CURRENT_SOURCE_DIR}/packages/Trajectory_Hotspots/Trajectory_Hotspots/
)

# Create the executable target and specify the source files
add_executable(Visualization ${cppfiles})

# Link CGAL libraries to the target
target_link_libraries(Visualization Qt5::Widgets CGAL::CGAL)

if(CGAL_Qt5_FOUND)
  # Link with the required CGAL libraries
  target_link_libraries(Visualization CGAL::CGAL_Basic_viewer opengl32)
endif()

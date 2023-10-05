# Minimum CMake version
cmake_minimum_required(VERSION 3.1...3.23)

# Project name
project(Visualization)

# Find CGAL and optional Qt5 component
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

# Create a list of source files in the "Trajectory-Hotspots" directory
file(
  GLOB_RECURSE cppfiles
  ${CMAKE_CURRENT_SOURCE_DIR}/Trajectory-Hotspots/**.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Trajectory-Hotspots/**.h
)

# Create the executable target and specify the source files
add_executable(Visualization ${cppfiles})

# Link CGAL libraries to the target
target_link_libraries(Visualization CGAL::CGAL)

if(CGAL_Qt5_FOUND)
  # Link with the required CGAL libraries
  target_link_libraries(Visualization CGAL::CGAL_Basic_viewer)
endif()

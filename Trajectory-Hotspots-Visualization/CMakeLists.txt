# Project name
project(Trajectory-Hotspots-Visualization)

# Find CGAL and optional Qt5 component
find_package(CGAL REQUIRED Qt5)
find_package(OpenGL REQUIRED)

# Recursive globbing to find all files in a directory
file(
  GLOB_RECURSE cppfiles
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

set(TRAJECTORY_HOTSPOTS_DIR ${PACKAGE_DIR}/Trajectory_Hotspots/Trajectory_Hotspots/Trajectory_Hotspots)

file(GLOB packagefiles
  ${TRAJECTORY_HOTSPOTS_DIR}/aabb.cpp
  ${TRAJECTORY_HOTSPOTS_DIR}/float.cpp
  ${TRAJECTORY_HOTSPOTS_DIR}/segment.cpp
  ${TRAJECTORY_HOTSPOTS_DIR}/segment_search_tree.cpp
  ${TRAJECTORY_HOTSPOTS_DIR}/trajectory.cpp
  ${TRAJECTORY_HOTSPOTS_DIR}/vec2.cpp
  ${TRAJECTORY_HOTSPOTS_DIR}/trapeziodal_map.cpp
)

# Include lib
include_directories(
${CGAL_INCLUDE_DIRS}
${QT5_INCLUDE_DIRS}
${TRAJECTORY_HOTSPOTS_DIR}/../
)

# Create the executable target and specify the source files
add_executable(${PROJECT_NAME} ${cppfiles} ${packagefiles})

# Link CGAL libraries to the target
target_link_libraries(${PROJECT_NAME} Qt5::Widgets CGAL::CGAL)

# Link OpenGL libraries to the target
if(CGAL_Qt5_FOUND)
  # Link with the required CGAL libraries and OpenGL32
  target_link_libraries(${PROJECT_NAME} CGAL::CGAL_Basic_viewer opengl32)
endif()

# Precompile header
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
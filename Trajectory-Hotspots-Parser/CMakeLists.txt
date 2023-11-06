# Project name
project(Trajectory-Hotspots-Parser)

# Recursive globbing to find all files in a directory
file(
  GLOB_RECURSE cppfiles
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

# Create the library target and specify the source files
add_library(${PROJECT_NAME}_lib ${cppfiles} ${TRAJECTORY_HOTSPOTS_PACKAGE_CPP_FILES} ${TRAJECTORY_HOTSPOTS_PACKAGE_CPP_FILES} pch.h)
target_include_directories(${PROJECT_NAME}_lib
        PUBLIC
        ..
        ${PROJECT_SOURCE_DIR}/
        ${TRAJECTORY_HOTSPOTS_SOLUTION_DIR}/)
target_precompile_headers(${PROJECT_NAME}_lib PUBLIC pch.h)
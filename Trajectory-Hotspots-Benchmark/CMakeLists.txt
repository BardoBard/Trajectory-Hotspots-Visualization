# Project name
project(Trajectory-Hotspots-Benchmark)

include(FetchContent)

FetchContent_Declare(

    nanobench

    GIT_REPOSITORY https://github.com/martinus/nanobench.git

    GIT_TAG v4.1.0

    GIT_SHALLOW TRUE)


FetchContent_MakeAvailable(nanobench)

# Recursive globbing to find all files in a directory
file(
  GLOB_RECURSE cppfiles
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

# Create the executable target and specify the source files
add_executable(${PROJECT_NAME} ${cppfiles})

# Link necessary libraries and include directories
target_link_libraries(${PROJECT_NAME} PRIVATE Trajectory-Hotspots-Visualization_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE Trajectory-Hotspots-Parser_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE nanobench)

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../templates DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
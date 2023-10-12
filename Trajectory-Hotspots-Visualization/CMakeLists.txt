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

# Include lib
include_directories(
${CGAL_INCLUDE_DIRS}
${QT5_INCLUDE_DIRS}
)

# Create the executable target and specify the source files
add_executable(${PROJECT_NAME} ${cppfiles})

# Link CGAL libraries to the target
target_link_libraries(${PROJECT_NAME} Qt5::Widgets CGAL::CGAL)

# Link OpenGL libraries to the target
if(CGAL_Qt5_FOUND)
  # Link with the required CGAL libraries and OpenGL32
  target_link_libraries(${PROJECT_NAME} CGAL::CGAL_Basic_viewer opengl32)
endif()

# Precompile header
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
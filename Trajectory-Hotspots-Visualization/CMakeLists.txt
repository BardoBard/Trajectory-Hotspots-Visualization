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

# copy examples to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../examples/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/)
     
# copy examples to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../docs/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/docs/)

# copy examples to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../etc/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/etc/)

# Create the executable target and specify the source files
add_executable(${PROJECT_NAME} ${cppfiles})

# Create the library target and specify the source files
add_library(${PROJECT_NAME}_lib ${cppfiles} pch.h)

# Include lib
include_directories(
        PUBLIC
        ${CGAL_INCLUDE_DIRS}
        ${QT5_INCLUDE_DIRS}
        ${TRAJECTORY_HOTSPOTS_SOLUTION_DIR}/
)
# Include lib
target_include_directories(${PROJECT_NAME}_lib
        PUBLIC
        ..
        ${PROJECT_SOURCE_DIR}/
        ${CGAL_INCLUDE_DIRS}
        ${QT5_INCLUDE_DIRS}
        ${TRAJECTORY_HOTSPOTS_SOLUTION_DIR}/)


# Link CGAL libraries to the target
target_link_libraries(${PROJECT_NAME} Qt5::Widgets CGAL::CGAL Trajectory-Hotspots-Parser_lib)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC Qt5::Widgets CGAL::CGAL Trajectory-Hotspots-Parser_lib)

# Link OpenGL libraries to the target
if (CGAL_Qt5_FOUND)
    # Link with the required CGAL libraries and OpenGL32
    target_link_libraries(${PROJECT_NAME} CGAL::CGAL_Basic_viewer opengl32)
    target_link_libraries(${PROJECT_NAME}_lib PUBLIC CGAL::CGAL_Basic_viewer opengl32)
endif ()

# Precompile header
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
target_precompile_headers(${PROJECT_NAME}_lib PRIVATE pch.h)
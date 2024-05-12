cmake_minimum_required(VERSION 3.10)
project(main)

# Find required frameworks
find_library(AVFOUNDATION AVFoundation)
find_library(FOUNDATION Foundation)

# Set C++ standard and libc++ as the standard library
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

set(EIGEN3_INCLUDE_DIR "./libraries/eigen-3.4.0")
include_directories(${EIGEN3_INCLUDE_DIR})

set(OpenCV_DIR "/opt/homebrew/Cellar/opencv/4.9.0_8.reinstall/lib/cmake/opencv4") # Path to OpenCVConfig.cmake
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add the executable
add_executable(main src/main.cpp src/Wrapper.mm src/utils.cpp)

# Include directories for header files
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Link the required frameworks
target_link_libraries(main ${AVFOUNDATION} ${FOUNDATION})

# Link OpenCV
target_link_libraries(main ${OpenCV_LIBS})
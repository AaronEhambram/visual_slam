cmake_minimum_required(VERSION 3.10)
project(visual_slam)

# Set build type for debugging if not specified
#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Debug)
#endif()

set(OpenCV_DIR "/home/ehambram/workspaces/libraries/opencv/opencv-4.x/build")
find_package(OpenCV REQUIRED)

set(GTSAM_DIR "/home/ehambram/workspaces/libraries/gtsam/build")
find_package(GTSAM REQUIRED)

find_package (Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
# Ensure debug flags are included
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

include_directories(${OpenCV_INCLUDE_DIRS} ${GTSAM_INCLUDE_DIR} include) 
add_executable(visual_slam src/main.cpp src/VisualSlam.cpp src/SensorDataReader.cpp) 
target_link_libraries(visual_slam ${OpenCV_LIBS} gtsam ${CMAKE_THREAD_LIBS_INIT})
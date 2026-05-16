cmake_minimum_required(VERSION 3.10)
project(visual_slam)

set(OpenCV_DIR "/home/ehambram/workspaces/libraries/opencv/opencv-4.x/build")
find_package(OpenCV REQUIRED)

set(GTSAM_DIR "/home/ehambram/workspaces/libraries/gtsam/build")
find_package(GTSAM REQUIRED)

find_package (Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${OpenCV_INCLUDE_DIRS} ${GTSAM_INCLUDE_DIR} include) 
add_executable(visual_slam src/main.cpp src/VisualSlam.cpp) 
target_link_libraries(visual_slam ${OpenCV_LIBS} gtsam ${CMAKE_THREAD_LIBS_INIT})
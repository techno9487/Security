cmake_minimum_required(VERSION 2.8)

project(security)

add_executable(project main.cpp electronics.cpp)
target_link_libraries(project wiringPi opencv_highgui opencv_core opencv_video opencv_imgproc)

include_directories("/usr/local/include")


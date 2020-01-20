cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 14)

project(image_utils)

add_executable(image_blur src/main.cpp src/image.cpp)
target_link_libraries(image_blur jpeg)

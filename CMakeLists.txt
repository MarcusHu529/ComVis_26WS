cmake_minimum_required(VERSION 3.20)

project(ComVis_26WS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(opencv_test main.cpp)

target_link_libraries(opencv_test PRIVATE ${OpenCV_LIBS})
target_include_directories(opencv_test PRIVATE ${OpenCV_INCLUDE_DIRS})

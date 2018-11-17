cmake_minimum_required(VERSION 2.8.3)
project(pedestrian_mu)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native")
set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV)
include_directories("/usr/local/include/eigen3")
include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp imageProcess.cpp SVMnode.cpp SVMnode.h)
target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        )
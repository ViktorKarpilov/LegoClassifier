cmake_minimum_required(VERSION 4.1)
project(LegoClassifierUtils)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.12.0
        GIT_SHALLOW TRUE
)
FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v3.0.2
        GIT_SHALLOW TRUE
)
#FetchContent_Declare(
#        OpenCV
#        GIT_REPOSITORY https://github.com/opencv/opencv.git
#        GIT_TAG 4.13.0
#        GIT_SHALLOW TRUE
#)

FetchContent_MakeAvailable(Catch2 pybind11)

find_package(OpenCV REQUIRED)

option(ENABLE_TESTING "Guess what ? For testing !" OFF)
message(${ENABLE_TESTING})

add_subdirectory(LegoContours)

#add_library(LegoClassifierUtils SHARED library.cpp)

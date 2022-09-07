cmake_minimum_required(VERSION 3.22)
project(hyper_grapher)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_library(hyper_grapher_lib)
add_subdirectory(core)

target_link_libraries(hyper_grapher_lib PUBLIC Qt5::Widgets)

add_executable(hyper_grapher main.cpp)

target_link_libraries(hyper_grapher PUBLIC Qt5::Core Qt5::Widgets hyper_grapher_lib)
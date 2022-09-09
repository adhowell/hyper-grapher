cmake_minimum_required(VERSION 3.22)
project(hyper_grapher)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(core)
add_subdirectory(gui)

add_executable(hyper_grapher main.cpp)

target_link_libraries(hyper_grapher PUBLIC core_lib gui_lib)